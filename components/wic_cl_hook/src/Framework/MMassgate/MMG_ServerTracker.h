// Massgate
// Copyright (C) 2017 Ubisoft Entertainment
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// THIS FILE HAS BEEN MODIFIED FOR USE IN A NON-UBISOFT PROJECT. IT IS NOT
// AN ORIGINAL COPY.
//
#pragma once

#include <MCommon2/MC_GrowingArray.h>
#include <MCommon2/MC_HashMap.h>
#include <MCommon2/MC_HybridArray.h>
#include <MCommon2/MC_SortedGrowingArray.h>
#include <MNetwork/MN_UdpSocket.h>
#include "MMG_Constants.h"
#include "MMG_LadderProtocol.h"
#include "MMG_ServerVariables.h"

class MN_ReadMessage;
class MN_WriteMessage;
class MMG_ServerFilter;
class MMG_ContentDownloader;
class MMG_IClanMatchHistoryListener;
class MMG_IStatsListerner;
class MMG_Messaging;
class MMG_MasterConnection;

namespace MMG_Clan { class Description; }
namespace MMG_DecorationProtocol { class PlayerMedalsRsp; class PlayerBadgesRsp; class ClanMedalsRsp; }
namespace MMG_FriendsLadderProtocol { class FriendsLadderRsp; }
namespace MMG_MiscClanProtocols { class TopTenWinningStreakRsp; }
namespace MMG_MiscClanProtocols { class TopTenWinningStreakRsp; }

class MMG_IServerTrackerListener
{
public:
	virtual bool ReceiveMatchingServer(const MMG_TrackableServerMinimalPingResponse& aServer) = 0;
	virtual bool ReceiveExtendedGameInfo(const MMG_TrackableServerFullInfo& aServer, void *someData, u32 someDataLen) = 0;
	virtual void ReceiveInviteToServer(const u32 aServerId, const u32 aConnectCookie) = 0;
	virtual void ReceiveServerListEntry() { }
	virtual void ReceiveNumTotalServers(const u32 aNumTotalServers) { }
};

class MMG_IPlayerLadderListener
{
public:
	virtual void ReceiveLadder(MMG_LadderProtocol::LadderRsp& aLadderRsp) = 0;
};

class MMG_IClanLadderListener
{
public:
	virtual bool ReceiveClan(const u32 aLadderPos, const u32 aMaxLadderPos, const u32 aScore, const MMG_Clan::Description& aClan) = 0;
	virtual bool OnNoMoreLadderResults() = 0;
	virtual bool ReceiveTopTenClans(MMG_MiscClanProtocols::TopTenWinningStreakRsp& aTopTenRsp) = 0;
};

class MMG_IFriendsLadderListener
{
public:
	virtual bool ReceiveFriendsLadder(MMG_FriendsLadderProtocol::FriendsLadderRsp& aLadderRsp) = 0;
};

class MMG_IDecorationListener
{
public:
	virtual void ReceivePlayerMedals(MMG_DecorationProtocol::PlayerMedalsRsp& aMedalRsp) = 0;
	virtual void ReceivePlayerBadges(MMG_DecorationProtocol::PlayerBadgesRsp& aBadgeRsp) = 0;
	virtual void ReceiveClanMedals(MMG_DecorationProtocol::ClanMedalsRsp& aMedalRsp) = 0;
};

class MMG_ServerTracker
{
public:
	static MMG_ServerTracker *GetInstance();

	// Tournament matches
	class TournamentServerListener
	{
	public:
		virtual void ReceiveServerForTournamentMatch(const MMG_TrackableServerFullInfo& aServer, u32 thePassword) = 0;
		virtual void OnNoServerFound() = 0;
	};

	// Content Download thingys
	struct CycleCacheEntry
	{
		u64 myCycleHash;
		MC_GrowingArray<u64> myMapHashList;
	};

	void PrivHandlePingers();

private:
	MC_GrowingArray<MMG_IServerTrackerListener *> myListeners;
	TournamentServerListener* myCurrentTournamentServerListener;
	MC_HybridArray<MMG_IPlayerLadderListener*, 4> myPlayerLadderListeners;

	// MiniInfo and ExtendedGameInfo correspond to the boolean values false and
	// true, respectively, in the original code. Do not change the order.
	enum class PingRequestType : u8
	{
		MiniInfo = 0,
		ExtendedGameInfo = 1,
		TimeCriticalPing = 2,
		ModDataDownload = 3,
	};

	struct PingRequest
	{
		u32 myIp = 0;
		u16 myMassgateCommPort = 0;
#if 0
		bool myGetExtendedInfoFlag = false;
#else
		PingRequestType myRequestType = PingRequestType::MiniInfo;
#endif
	};

	MC_GrowingArray<PingRequest> myPingRequests;

	struct Pinger
	{
		u64 myStartTimeStamp = 0;
		u8 mySequenceNumber = 0;
#if 0
		bool myGetExtendedInfoFlag = false;
#else
		PingRequestType myRequestType = PingRequestType::MiniInfo;
#endif
		SOCKADDR_IN myTarget = {};
	};

	struct PingMeasure
	{
		u32 ipNumber = 0;
		u32 lowestPing = 0;
		bool operator==(const PingMeasure& aRhs) const { return ipNumber == aRhs.ipNumber; }
		bool operator<(const PingMeasure& aRhs) const { return ipNumber < aRhs.ipNumber; }
		bool operator>(const PingMeasure& aRhs) const { return ipNumber > aRhs.ipNumber; }
	};

	MC_GrowingArray<Pinger>	myCurrentPingers;
	MC_GrowingArray<PingMeasure> myPingMeasures;
	MN_UdpSocket *myPingSocket;
	u8 myBaseSequenceNumber;
	MC_HashMap<u64, CycleCacheEntry*> myCycleCache;
	MC_GrowingArray<MMG_TrackableServerMinimalPingResponse> myPingedServersWithUnknownCycle;

	MN_WriteMessage& myOutputMessage; // Only one. Cannot queue messages.

	struct LadderItem
	{
		u32 profileId;
		u32 pos;
		u32 maxPos;
		u32 score;
		bool operator<(const LadderItem& aRhs) const { return profileId < aRhs.profileId; }
		bool operator>(const LadderItem& aRhs) const { return profileId > aRhs.profileId; }
		bool operator== (const LadderItem& aRhs) const { return profileId == aRhs.profileId; }
	};

	MC_SortedGrowingArray<LadderItem> myCurrentPageLadderItems;
	MMG_IClanLadderListener* myCurrentClanLadderListener;
	MMG_IFriendsLadderListener* myCurrentFriendsLadderListener;

	bool myHasBeenInitialised;
	bool myIsGettingServerlist;
	u32 myLastSearchForServersTime;
	bool myHasEverReceivedPong;
	bool myHasGottenListOfServers;
	u32 myGameVersion;
	u32 myGameProtocolVersion;

	MMG_IDecorationListener *myDecorationListener;
	MMG_IStatsListerner *myStatsListener;

	MC_HybridArray<MMG_TrackableServerFullInfo, 16> myPingResponsesThatWaitForCycleHash;
	MC_HashMap<u32, MMG_GameNameString> myServerToNameMap;
	MC_HashMap<u32, ServerType> myServerToServerTypeMap;
	MC_HashMap<u32, u16> myServerToCommPortMap;

	MMG_LadderProtocol::LadderRsp myCurrentChunkedLadderResponse;

	u32 myTimeOfLastPing;

	MC_HybridArray<MMG_IClanMatchHistoryListener *, 2> myClanMatchHistoryListeners;

	void PrivDispatchPingerMessage(Pinger& Pinger);
	void PrivHandlePingerListeners();
	void PrivHandlePingerResends();
	void PrivHandlePingerResponses();
	u32 GetLowestPing(u32 currentTime, const Pinger& currentPinger);
};
static_assert_size(MMG_ServerTracker, 0x5A48);