#include <Hooks/Memory.h>
#include <MCommon2/MI_Time.h>
#include <MNetwork/MN_LoopbackConnection.h>
#include <MNetwork/MN_ReadMessage.h>
#include <MNetwork/MN_UdpSocket.h>
#include <MNetwork/MN_WriteMessage.h>
#include <WinSock2.h>
#include <mutex>
#include "MMG_ModDataDownloader.h"
#include "MMG_ProtocolDelimiters.h"
#include "MMG_TrackableServer.h"

constexpr u16 MASSGATE_GAME_SERVER_QUERY_PORT = 22993;
constexpr u16 MMG_NETWORK_PORT_RANGE_SIZE = 5000;

#if WIC_MP
u16 MMG_TRACKABLE_SERVER_QUERY_PORT = MASSGATE_GAME_SERVER_QUERY_PORT;
#elif WIC_DS
DeclareGlobalRef(u16, MMG_TRACKABLE_SERVER_QUERY_PORT, 0x0087312C);
#endif

MMG_TrackableServer::ThreadedPingHandler::ThreadedPingHandler()
{
	static u32 lastSuccessfulBind = MMG_TRACKABLE_SERVER_QUERY_PORT;

	for (u32 serverIndex = 0; !myServingSocket;)
	{
		myServingSocket = MN_UdpSocket::Create(false);
		myListeningPort = lastSuccessfulBind + serverIndex;

		if (!myServingSocket)
			break;

		if (!myServingSocket->Bind(static_cast<u16>(myListeningPort)))
		{
			MN_UdpSocket::Destroy(myServingSocket);

			if (serverIndex++ >= MMG_NETWORK_PORT_RANGE_SIZE)
			{
				// Failed to bind and we tried all ports in the range. Bail out.
				break;
			}
		}
	}

	if (!myServingSocket)
	{
		MC_ERROR("Failed to bind to a port between %hu-%u. SHUTTING DOWN.", MMG_TRACKABLE_SERVER_QUERY_PORT, myListeningPort);
		myListeningPort = 0;
	}
	else
	{
		lastSuccessfulBind = myListeningPort;
	}
}

MMG_TrackableServer::ThreadedPingHandler::~ThreadedPingHandler()
{
	MN_UdpSocket::Destroy(myServingSocket);
}

void MMG_TrackableServer::ThreadedPingHandler::Run()
{
	while (!StopRequested() && myServingSocket && HandleClientRequests())
	{
		fd_set readfds;
		FD_ZERO(&readfds);
		FD_SET(myServingSocket->myWinsock, &readfds);
		
		TIMEVAL timeout =
		{
			.tv_sec = 1,
			.tv_usec = 0,
		};
		
		if (select(0, &readfds, nullptr, nullptr, &timeout) == SOCKET_ERROR)
			MC_DEBUG("Select(myServingSocket) failed: %d", WSAGetLastError());
	}
}

bool MMG_TrackableServer::ThreadedPingHandler::HandleClientRequests()
{
	if (!myServingSocket)
		return false;

	// Loop until we run out of data
	SOCKADDR_IN recvFrom = {};
	char recvBuffer[8192];
	s32 recvLen = 0;

	while (true)
	{
		if (!myServingSocket->RecvBuffer(recvFrom, recvBuffer, sizeof(recvBuffer), recvLen))
			return false;

		if (recvLen <= 0)
			break;

		// Parse the incoming message
		MN_ReadMessage rm;
		rm.SetLightTypecheckFlag(true);

		if (rm.BuildMe(recvBuffer, recvLen) != recvLen)
		{
			// Normally it'd be a good idea to log an error, but port-scanning bots like to spam us
			// with garbage
			break;
		}
		
		// Closed MP Alpha, least MTU for a player was 1300
		MN_WriteMessage wm(1024);

		while (!rm.AtEnd())
		{
			MN_DelimiterType delim = 0;
			if (!rm.ReadDelimiter(delim))
			{
				MC_DEBUG("Protocol error 1 from peer %s", IpToString(recvFrom.sin_addr.s_addr));
				break;
			}

			switch (delim)
			{
			case MMG_ProtocolDelimiters::SERVERTRACKER_USER_MINIPINGREQUEST:
			{
				if (u8 sequence = 0; rm.ReadUChar(sequence))
				{
					wm.WriteDelimiter(MMG_ProtocolDelimiters::SERVERTRACKER_USER_MINIPINGRESPONSE);
					wm.WriteUChar(sequence);

					std::scoped_lock lock(myMutex);
					copyMinimalPingResponse.ToStream(wm);
				}
				else
				{
					MC_DEBUG("Protocol error 2 from peer %s", IpToString(recvFrom.sin_addr.s_addr));
					break;
				}
			}
			break;

			case MMG_ProtocolDelimiters::SERVERTRACKER_USER_GAME_EXTENDED_INFO_REQUEST:
			{
				wm.WriteDelimiter(MMG_ProtocolDelimiters::SERVERTRACKER_USER_GAME_EXTENDED_INFO_RESPONSE);

				std::scoped_lock lock(myMutex);
				copyFullServerInfo.ToStream(wm);

				wm.WriteInt(extendedGameInfoLength);

				if (extendedGameInfoLength > 0)
					wm.WriteRawData(extendedGameInfoData, extendedGameInfoLength);
			}
			break;

			case MMG_ProtocolDelimiters::CUSTOM_SERVERTRACKER_TIME_CRITICAL_PING:
			{
				// todo: implement
			}
			break;

			case MMG_ProtocolDelimiters::CUSTOM_SERVERTRACKER_MOD_DATA_DOWNLOAD_REQ:
			{
				MMG_ModDataDownloader::Request req;

				if (req.FromStream(rm))
					MMG_ModDataDownloader::Server::HandleClientRequest(recvFrom.sin_addr.s_addr, recvFrom.sin_port, req);
			}
			break;

			default:
			{
				// NOTE: A bug has been fixed here. The original print didn't have delim passed in and would crash
				// or log incorrect values.
				MC_DEBUG("Got unknown command %hu from peer %s", delim, IpToString(recvFrom.sin_addr.s_addr));
			}
			break;
			}
		}

		// Send the response if applicable
		if (wm.GetMessageSize() > 0)
		{
			MN_LoopbackConnection<1024> lbc;

			if (wm.SendMe(&lbc) == MN_CONN_OK)
				myServingSocket->SendBuffer(recvFrom, lbc.GetData(), lbc.GetDataLength());
		}
	}

	return true;
}

bool MMG_TrackableServer::Update()
{
#if WIC_MP
	return ((bool(__stdcall *)(MMG_TrackableServer *))0x00799490)(this);
#elif WIC_DS
	return ((bool(__thiscall *)(MMG_TrackableServer *))0x00482A30)(this);
#else
#error Unimplemented
#endif
}

MMG_TrackableServer *MMG_TrackableServer::GetInstance()
{
	return ourInstance;
}

const char *MMG_TrackableServer::IpToString(u32 aIp)
{
	thread_local char buf[32];

	sprintf_s(buf, "%u.%u.%u.%u", aIp & 0xFF, (aIp >> 8) & 0xFF, (aIp >> 16) & 0xFF, (aIp >> 24) & 0xFF);
	return buf;
}