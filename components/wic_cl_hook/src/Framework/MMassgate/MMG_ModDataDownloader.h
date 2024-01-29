#pragma once

#include <vector>

class MN_ReadMessage;
class MN_WriteMessage;

class MMG_ModDataDownloader
{
public:
	class Request
	{
	public:
		u32 myFileId = 0;
		u32 myFileDataOffset = 0;
		u16 myFileDataSize = 0;

		void ToStream(MN_WriteMessage& Message) const;
		bool FromStream(MN_ReadMessage& Message);

		u32 RemainingSize() const
		{
			return myFileDataSize - myFileDataOffset;
		}
	};

	class Response
	{
	public:
		u32 myFileId = 0;
		u32 myFileDataHash = 0;
		u32 myFileDataOffset = 0;
		u16 myFileDataSize = 0;
		u8 myFileData[4096] = {};

		void ToStream(MN_WriteMessage& Message) const;
		bool FromStream(MN_ReadMessage& Message);
	};

private:
	struct ClientSideRequestInfo
	{
		u32 myLastUpdateTime = 0;
		void *myFileHandle = nullptr;
		Request myRequest;
	};

	struct ServerSideRequestInfo
	{
		u32 myIp = 0;
		u16 myPort = 0;
		Request myRequest;
	};

public:
	class Client
	{
	private:
		static inline u32 myServerIp = 0;
		static inline u16 myServerPort = 0;
		inline static std::vector<ClientSideRequestInfo> myFilesPendingDownloadFromServer;

	public:
		static void SetupFileRequests(u32 ServerIp, u16 ServerPort, const std::vector<Request>& Requests);
		static bool HandleServerResponse(const Response& Response);
		static bool GetNextServerRequest(u32& Ip, u16& Port, Request& Request);
	};

	class Server
	{
	private:
		inline static std::vector<ServerSideRequestInfo> myPendingClientRequests;

	public:
		static void SetupFilesToServe(const std::vector<Request>& Requests);
		static bool HandleClientRequest(u32 Ip, u16 Port, const Request& Request);
		static bool GetNextClientResponse(u32& Ip, u16& Port, Response& Response);
	};
};