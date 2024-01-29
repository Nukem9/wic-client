#include <MCommon2/MI_Time.h>
#include <MNetwork/MN_ReadMessage.h>
#include <MNetwork/MN_WriteMessage.h>
#include "MMG_Tiger.h"
#include "MMG_ModDataDownloader.h"

void MMG_ModDataDownloader::Request::ToStream(MN_WriteMessage& Message) const
{
	Message.WriteUInt(myFileId);
	Message.WriteUInt(myFileDataOffset);
	Message.WriteUShort(myFileDataSize);
}

bool MMG_ModDataDownloader::Request::FromStream(MN_ReadMessage& Message)
{
	return Message.ReadUInt(myFileId) &&
		Message.ReadUInt(myFileDataOffset) &&
		Message.ReadUShort(myFileDataSize);
}

void MMG_ModDataDownloader::Response::ToStream(MN_WriteMessage& Message) const
{
	Message.WriteUInt(myFileId);
	Message.WriteUInt(myFileDataHash);
	Message.WriteUInt(myFileDataOffset);
	Message.WriteUShort(myFileDataSize);

	if (myFileDataSize > 0)
		Message.WriteRawData(myFileData, std::min<u16>(myFileDataSize, sizeof(myFileData)));
}

bool MMG_ModDataDownloader::Response::FromStream(MN_ReadMessage& Message)
{
	bool good = Message.ReadUInt(myFileId);
	good = good && Message.ReadUInt(myFileDataHash);
	good = good && Message.ReadUInt(myFileDataOffset);
	good = good && Message.ReadUShort(myFileDataSize);

	if (good && myFileDataSize > 0)
	{
		// We'll read as much as we can, but never more than the buffer size. This allows for
		// the server to send more data than the client expects, which may be useful later.
		myFileDataSize = std::min<u16>(myFileDataSize, sizeof(myFileData));

		good = good && Message.ReadRawData(myFileData, myFileDataSize);
	}

	return good;
}

void MMG_ModDataDownloader::Client::SetupFileRequests(u32 ServerIp, u16 ServerPort, const std::vector<Request>& Requests)
{
	myServerIp = ServerIp;
	myServerPort = ServerPort;

	for (const auto& r : Requests)
	{
		myFilesPendingDownloadFromServer.push_back(ClientSideRequestInfo
		{
			.myLastUpdateTime = MI_Time::GetSystemTime(),
			.myFileHandle = nullptr,
			.myRequest = r,
		});
	}
}

bool MMG_ModDataDownloader::Client::HandleServerResponse(const Response& Response)
{
	auto itr = std::find_if(myFilesPendingDownloadFromServer.begin(), myFilesPendingDownloadFromServer.end(), [&](const auto& r)
	{
		return r.myRequest.myFileId == Response.myFileId;
	});

	if (itr == myFilesPendingDownloadFromServer.end())
	{
		// Not found. Why did the server send us this?
		return false;
	}

	// Matching file found. Validate it.
	if (Response.myFileDataOffset != itr->myRequest.myFileDataOffset ||
		Response.myFileDataSize <= 0)
		return false;

	// Check if the data sent over the network was intact
	const auto sizeToCopy = std::min<u32>(itr->myRequest.RemainingSize(), Response.myFileDataSize);
	const auto calculatedHash = MMG_Tiger().GenerateHash(Response.myFileData, Response.myFileDataSize).Get32BitSubset();

	if (calculatedHash != Response.myFileDataHash)
		return false;

	itr->myLastUpdateTime = MI_Time::GetSystemTime();
	itr->myRequest.myFileDataOffset += sizeToCopy;
	// TODO: Write

	if (itr->myRequest.RemainingSize() <= 0)
	{
		MC_DEBUG("Completed download of file %u", itr->myRequest.myFileId);
		myFilesPendingDownloadFromServer.erase(itr);
	}

	return true;
}

bool MMG_ModDataDownloader::Client::GetNextServerRequest(u32& Ip, u16& Port, Request& Request)
{
	if (myFilesPendingDownloadFromServer.empty())
		return false;

	// Just pick the first one
	Ip = myServerIp;
	Port = myServerPort;
	Request = myFilesPendingDownloadFromServer[0].myRequest;

	return true;
}

void MMG_ModDataDownloader::Server::SetupFilesToServe(const std::vector<Request>& Requests)
{
}

bool MMG_ModDataDownloader::Server::HandleClientRequest(u32 Ip, u16 Port, const Request& Request)
{
	// Drop excessive requests
	if (myPendingClientRequests.size() >= 64)
		return false;

	const auto requestsPerThisEndpoint = std::count_if(myPendingClientRequests.begin(), myPendingClientRequests.end(), [&](const auto& r)
	{
		return r.myIp == Ip && r.myPort == Port;
	});

	if (requestsPerThisEndpoint >= 4)
		return false;

	myPendingClientRequests.push_back({ Ip, Port, Request });
	return true;
}

bool MMG_ModDataDownloader::Server::GetNextClientResponse(u32& Ip, u16& Port, Response& Response)
{
	return false;
}
