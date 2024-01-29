#include <Hooks/Memory.h>
#include <MCommon2/MI_Time.h>
#include <MNetwork/MN_ReadMessage.h>
#include <MNetwork/MN_WriteMessage.h>
#include <MNetwork/MN_LoopbackConnection.h>
#include <MCommon2/MC_CommandLine.h>
#include <optional>
#include "MMG_ModDataDownloader.h"
#include "MMG_Options.h"
#include "MMG_ProtocolDelimiters.h"
#include "MMG_TrackableServer.h"
#include "MMG_ServerTracker.h"

static DeclareGlobalRef(MMG_ServerTracker *, myInstance, SelectOffset(0x00E30D60, 0x008F31CC));

MMG_ServerTracker *MMG_ServerTracker::GetInstance()
{
	return myInstance;
}

void MMG_ServerTracker::PrivDispatchPingerMessage(Pinger& Pinger)
{
	MN_LoopbackConnection<1024> loopbackConnection;
	MN_WriteMessage wm(loopbackConnection.GetRecommendedBufferSize());

	switch (Pinger.myRequestType)
	{
	case PingRequestType::MiniInfo:
	{
		wm.WriteDelimiter(MMG_ProtocolDelimiters::SERVERTRACKER_USER_MINIPINGREQUEST);
		wm.WriteUChar(Pinger.mySequenceNumber);
	}
	break;

	case PingRequestType::ExtendedGameInfo:
	{
		wm.WriteDelimiter(MMG_ProtocolDelimiters::SERVERTRACKER_USER_GAME_EXTENDED_INFO_REQUEST);
	}
	break;

	case PingRequestType::TimeCriticalPing:
	{
	}
	break;

	case PingRequestType::ModDataDownload:
	{
		MMG_ModDataDownloader::Request req;

		if (MMG_ModDataDownloader::Client::GetNextServerRequest((u32&)Pinger.myTarget.sin_addr.s_addr, Pinger.myTarget.sin_port, req))
		{
			wm.WriteDelimiter(MMG_ProtocolDelimiters::CUSTOM_SERVERTRACKER_MOD_DATA_DOWNLOAD_REQ);
			req.ToStream(wm);
		}
	}
	break;
	}

	if (wm.GetMessageSize() > 0)
	{
		wm.SendMe(&loopbackConnection);

		if (!myPingSocket->SendBuffer(Pinger.myTarget, loopbackConnection.GetData(), loopbackConnection.GetDataLength()))
		{
			MC_ERROR("Bad ping socket. Tearing down connection.");
			MN_UdpSocket::Destroy(myPingSocket);
		}
	}
}

void MMG_ServerTracker::PrivHandlePingerListeners()
{
	static u32 lastIpPinged = 0;

	if (!myPingSocket || myListeners.empty())
		return;

	// Spawn new pingers
	u32 maxPingsPerSecond = 10;
	u32 cmdPingsPerSecond = 10;

	if (int pps; MC_CommandLine::GetInstance()->GetIntValue("pingspersec", pps))
	{
		cmdPingsPerSecond = std::clamp<u32>(pps, 1, 100);
		maxPingsPerSecond = cmdPingsPerSecond;
	}
	else if (MMG_Options::GetInstance())
	{
		maxPingsPerSecond = MMG_Options::GetInstance()->myPingsPerSecond;
		cmdPingsPerSecond = maxPingsPerSecond;
	}

	// Multiply number of pings based on user's internet connection setting
	if (MMG_Options::GetInstance())
		maxPingsPerSecond = std::max<u32>(1, MMG_Options::GetInstance()->myInternetConnection) * maxPingsPerSecond;

	const auto maxPingsThisFrame = std::min(maxPingsPerSecond / cmdPingsPerSecond, u32((MI_Time::GetSystemTime() - myTimeOfLastPing) / 1000.0f * float(maxPingsPerSecond)));

	for (u32 numPingsThisFrame = 0;; numPingsThisFrame++)
	{
		if (numPingsThisFrame >= maxPingsThisFrame)
			break;

		if (myCurrentPingers.size() >= maxPingsPerSecond)
			break;

		if (myPingRequests.empty())
			break;

		const auto pingIndex = [&]() -> size_t
		{
			// Prefer to ping different servers each time, if we can
			for (size_t i = 0; i < myPingRequests.size(); i++)
			{
				if (myPingRequests[i].myIp != lastIpPinged)
					return i;
			}

			return 0;
		}();

		const auto& request = myPingRequests[pingIndex];
		lastIpPinged = request.myIp;

		Pinger pinger;
		pinger.myRequestType = request.myRequestType;
		pinger.mySequenceNumber = myBaseSequenceNumber;
		pinger.myStartTimeStamp = MI_Time::GetSystemTime();
		pinger.myTarget.sin_family = AF_INET;
		pinger.myTarget.sin_addr.s_addr = /*htonl*/(request.myIp);
		pinger.myTarget.sin_port = htons(request.myMassgateCommPort);

		MC_ASSERT(pinger.myTarget.sin_port != 0);
		MC_DEBUG("Ping to %s:%hu [%u]", inet_ntoa(pinger.myTarget.sin_addr), ntohs(pinger.myTarget.sin_port), pinger.mySequenceNumber);

		PrivDispatchPingerMessage(pinger);

		myTimeOfLastPing = pinger.myStartTimeStamp;
		myCurrentPingers.push_back(pinger);
		myPingRequests.RemoveItemAtIndex(pingIndex);
	}
}

void MMG_ServerTracker::PrivHandlePingerResends()
{
	const auto pingRetryCount = 3;
	const auto pingTimeoutMs = 1500;

	// Resend any pending pings that may have been lost
	for (size_t i = 0; i < myCurrentPingers.size(); i++)
	{
		auto& pinger = myCurrentPingers[i];

		if ((MI_Time::GetSystemTime() - pinger.myStartTimeStamp) < pingTimeoutMs)
			continue;

		// If we've already retried this ping too many times, give up	
		const auto maxRetrySequenceNumber = myBaseSequenceNumber + pingRetryCount;

		if (pinger.mySequenceNumber++ >= maxRetrySequenceNumber)
		{
			MC_DEBUG("Ping to %s:%hu timed out", inet_ntoa(pinger.myTarget.sin_addr), ntohs(pinger.myTarget.sin_port));
			myCurrentPingers.RemoveCyclicAtIndex(i--);

			continue;
		}

		// Resend the ping
		MC_DEBUG("Resend ping %u to %s:%hu", pinger.mySequenceNumber - myBaseSequenceNumber, inet_ntoa(pinger.myTarget.sin_addr), ntohs(pinger.myTarget.sin_port));
		pinger.myStartTimeStamp = MI_Time::GetSystemTime();

		switch (pinger.myRequestType)
		{
		// We don't resend time critical pings
		case PingRequestType::TimeCriticalPing:
			pinger.mySequenceNumber = maxRetrySequenceNumber;
			break;

		default:
			PrivDispatchPingerMessage(pinger);
			break;
		}
	}
}

void MMG_ServerTracker::PrivHandlePingerResponses()
{
	if (!myPingSocket)
		return;

	// Loop until we run out of data
	SOCKADDR_IN recvFrom;
	char recvBuffer[8192];
	s32 recvLen = 0;

	while (true)
	{
		const auto parsingStartTimestamp = MI_Time::GetSystemTime();

		if (!myPingSocket->RecvBuffer(recvFrom, recvBuffer, sizeof(recvBuffer), recvLen))
			break;

		if (recvLen <= 0)
			break;

		// Check if we were expecting a response from this IP
		const auto currentPinger = [&]() -> std::optional<Pinger>
		{
			for (size_t i = 0; i < myCurrentPingers.size(); i++)
			{
				if (myCurrentPingers[i].myTarget.sin_addr.s_addr != recvFrom.sin_addr.s_addr)
					continue;

				if (myCurrentPingers[i].myTarget.sin_port != recvFrom.sin_port)
					continue;

				auto p = myCurrentPingers[i];
				myCurrentPingers.RemoveCyclicAtIndex(i--);
				return p;
			}

			return std::nullopt;
		}();

		if (!currentPinger.has_value())
		{
			MC_DEBUG("Ignoring unknown response.");
			continue;
		}

		// Then parse the incoming message
		MN_ReadMessage rm;
		rm.SetLightTypecheckFlag(true);

		if (rm.BuildMe(recvBuffer, recvLen) != recvLen)
		{
			MC_DEBUG("Got incomplete request. Ignoring.");
			continue;
		}

		for (bool good = true; good && !rm.AtEnd();)
		{
			MN_DelimiterType cmd = 0;
			good = good && rm.ReadDelimiter(cmd);

			if (!good)
			{
				MC_DEBUG("Could not read command. Ignoring.");
				continue;
			}

			// Indicate that at lest one packet made it through the firewall
			myHasEverReceivedPong = true;

			switch (cmd)
			{
			case MMG_ProtocolDelimiters::SERVERTRACKER_USER_MINIPINGRESPONSE:
			{
				u8 sequence;
				MMG_TrackableServerMinimalPingResponse rsp;

				if (rm.ReadUChar(sequence) && rsp.FromStream(rm))
				{
					if (sequence != currentPinger->mySequenceNumber)
					{
						// Incorrect sequence number. Put it back in the list.
						myCurrentPingers.push_back(*currentPinger);

						continue;
					}

					rsp.myIp = currentPinger->myTarget.sin_addr.s_addr;
					rsp.myPing = GetLowestPing(parsingStartTimestamp, *currentPinger);
					rsp.myServerType = myServerToServerTypeMap[rsp.myServerId];
					rsp.myGameName = myServerToNameMap[rsp.myServerId];
					rsp.myMassgateCommPort = myServerToCommPortMap[rsp.myServerId];

					MC_DEBUG("Ping response %s:%hu %ums (%u)", inet_ntoa(recvFrom.sin_addr), ntohs(recvFrom.sin_port), rsp.myPing, currentPinger->mySequenceNumber - myBaseSequenceNumber);

					CycleCacheEntry *entry = nullptr;
					myCycleCache.GetIfExists(rsp.myCurrentCycleHash, entry);

					if (entry && !entry->myMapHashList.empty())
					{
						for (size_t j = 0; j < myListeners.size(); j++)
						{
							if (!myListeners[j]->ReceiveMatchingServer(rsp))
								myListeners.RemoveCyclicAtIndex(j--);
						}
					}
					else
					{
						// Wait for cycle information
						myPingedServersWithUnknownCycle.push_back(rsp);
					}
				}
				else
				{
					MC_DEBUG("Got garbled information. Ignoring.");
					good = false;
				}
			}
			break;

			case MMG_ProtocolDelimiters::SERVERTRACKER_USER_GAME_EXTENDED_INFO_RESPONSE:
			{
				MMG_TrackableServerFullInfo rsp;
				s32 numBytes = 0;
				u8 buffer[MMG_TrackableServer::MAX_EXTENDED_INFO_LENGTH] = {};

				if (rsp.FromStream(rm) &&
					rm.ReadInt(numBytes) &&
					(numBytes <= 0 || rm.ReadRawData(buffer, sizeof(buffer), &numBytes)))
				{
					rsp.myIp = currentPinger->myTarget.sin_addr.s_addr;
					rsp.myPing = GetLowestPing(parsingStartTimestamp, *currentPinger);
					rsp.myServerType = myServerToServerTypeMap[rsp.myServerId];
					rsp.myServerName = myServerToNameMap[rsp.myServerId];
					rsp.myMassgateCommPort = myServerToCommPortMap[rsp.myServerId];

					for (auto& listener : myListeners)
						listener->ReceiveExtendedGameInfo(rsp, buffer, numBytes);
				}
			}
			break;

			case MMG_ProtocolDelimiters::CUSTOM_SERVERTRACKER_TIME_CRITICAL_PONG:
			{
				// TODO: implement
			}
			break;

			case MMG_ProtocolDelimiters::CUSTOM_SERVERTRACKER_MOD_DATA_DOWNLOAD_RSP:
			{
				MMG_ModDataDownloader::Response rsp;

				if (rsp.FromStream(rm))
					MMG_ModDataDownloader::Client::HandleServerResponse(rsp);
			}
			break;

			default:
			{
				MC_DEBUG("Received unknown command %hu. Ignoring.", cmd);
				good = false;
			}
			break;
			}
		}
	}
}

void MMG_ServerTracker::PrivHandlePingers()
{
	if (!myPingSocket)
	{
		MC_DEBUG("Attempting to create ping socket...");

		myPingSocket = MN_UdpSocket::Create(false);

		if (!myPingSocket->Bind(0))
			MC_DEBUG("Could not bind ping socket");
	}

	PrivHandlePingerListeners();
	PrivHandlePingerResends();
	PrivHandlePingerResponses();
}

u32 MMG_ServerTracker::GetLowestPing(u32 currentTime, const Pinger& currentPinger)
{
	u32 lowestPing = currentTime - currentPinger.myStartTimeStamp;
	
	// MODIFIED: We're no longer going to use the lowest ping recorded. It's often misleading anyways.
	if (false)
	{
		PingMeasure pm;
		pm.ipNumber = currentPinger.myTarget.sin_addr.s_addr;
		int index = myPingMeasures.FindInSortedArray(pm);

		if (index != -1)
		{
			if (myPingMeasures[index].lowestPing > lowestPing)
			{
				myPingMeasures[index].lowestPing = lowestPing;
				myPingMeasures.Sort();
			}
			else
			{
				// Ping can only differ 10% from lowest to that IP.
				lowestPing = std::min(myPingMeasures[index].lowestPing + myPingMeasures[index].lowestPing / 8, lowestPing);
			}
		}
		else
		{
			myPingMeasures.push_back(PingMeasure
			{
				.ipNumber = currentPinger.myTarget.sin_addr.s_addr,
				.lowestPing = lowestPing
			});
			myPingMeasures.Sort();
		}
	}

	return lowestPing;
}