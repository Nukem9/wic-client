#pragma once

#include <Hooks/Memory.h>
#include <MCommon2/MC_EggClockTimer.h>
#include <MCommon2/MC_GrowingArray.h>
#include <MCommon2/MC_HybridArray.h>
#include <MCommon2/MT_Mutex.h>
#include <MCommon2/MT_Thread.h>
#include "MMG_CdKeyValidator.h"
#include "MMG_PCCProtocol.h"
#include "MMG_ServerVariables.h"

class MMG_MasterConnection;
class MN_UdpSocket;

class MMG_TrackableServer
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

public:
	const static u32 MAX_EXTENDED_INFO_LENGTH = 483;

	class Map
	{
	public:
		u64 myHash;
		MC_LocString myName;
	};
	static_assert_size(Map, 0x10);

	struct ChatMessage
	{
		u32 sender;
		MC_LocString message;
	};
	static_assert_size(ChatMessage, 0x8);

	class Callbacks;

	class ThreadedPingHandler : public MT_Thread
	{
		MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

	private:
		u64 myCycleHash = 0;

	public:
		MMG_TrackableServerFullInfo copyFullServerInfo;
		MMG_TrackableServerMinimalPingResponse copyMinimalPingResponse;
		char extendedGameInfoData[MAX_EXTENDED_INFO_LENGTH] = {};
		u32 extendedGameInfoLength = 0;
		MT_Mutex myMutex;
		MN_UdpSocket *myServingSocket = nullptr;
		u32 myListeningPort = 0;

		ThreadedPingHandler();
		~ThreadedPingHandler() override;
		void Run() override;
		bool HandleClientRequests();
	};
	static_assert_size(ThreadedPingHandler, 0x6B0);

private:
	static inline DeclareGlobalRef(MMG_TrackableServer *, ourInstance, SelectOffset(0x00E30D50, 0x008F2CF0));

	MC_GrowingArray<Callbacks *> myCallbackListeners;
	MMG_MasterConnection *myMasterConnection;
	u64 myTimeOfLastUpdateInSeconds;
	MMG_TrackableServerFullInfo myFullServerInfo;
	bool myIsStarted;
	u32 myConnectCookieBase;
	MMG_TrackableServerCookie myCookie;
	u32 myPublicId;
	u32 myConnectToMasterServerStartTime;
	ThreadedPingHandler *myPingHandler;
	MMG_TrackableServerHeartbeat myLastReportedHeartbeat;
	MC_GrowingArray<MMG_IPCCMessagingListener *> myPCCListeners;
	MMG_PCCProtocol::ClientToMassgateGetPCC myOutgoingPccRequests;
	MC_EggClockTimer mySendPCCTimer;
	u32 myCDKeySequenceNumber;
	MMG_CdKey::Validator::EncryptionKey myCDKeyEncryptionKey;
	bool myHandshakeSent;
	bool myHaveCDKeyInformation;
	bool myHaveSentQuizAnswer;
	bool myHasStartupVariables;
	MMG_ServerStartupVariables myStartupVariables;
	bool myStartupIsCompleted;
	bool myHaveSentMapList;
	u64 myOldestChatMessage;
	MC_HybridArray<ChatMessage, 64>	myChatMessages;
	MC_HybridArray<Map, 64> myMapList;
	bool myContainsPreorderMap;
	bool myServerIsFakeFull;
	MC_HybridArray<u32, 16> myClientsWaitingForASlot;
	MC_EggClockTimer myRequestBannedWordsTimer;
	MC_HybridArray<MC_StaticLocString<255>, 255> myBannedWords;
	bool myShouldClearBannedWords;

public:
	bool Update();

	static MMG_TrackableServer *GetInstance();
	static const char *IpToString(u32 aIp);
};
static_assert_size(MMG_TrackableServer, 0x20B68);