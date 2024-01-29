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

#include <MCommon2/MC_String.h>
#include <MCommon2/MC_StaticArray.h>
#include "MMG_Constants.h"
#include "MMG_IStreamable.h"

class MMG_Tiger;
class MN_ReadMessage;
class MN_WriteMessage;

enum class ServerType
{
	NORMAL_SERVER,
	MATCH_SERVER,
	FPM_SERVER,
	TOURNAMENT_SERVER,
	CLANMATCH_SERVER
};

class MMG_TrackableServerCookie
{
public:
	union
	{
		MC_StaticArray<u64, 2> contents;

		struct
		{
			u64 trackid;
			u64 hash;
		};
	};

	MMG_TrackableServerCookie();
	bool operator==(const MMG_TrackableServerCookie& aOther) const;

	bool MatchesSecret(const s64 theSecret, const s64 thePreviousSecret, const MMG_Tiger& aHasher) const;
};
static_assert_size(MMG_TrackableServerCookie, 0x10);

class MMG_TrackablePlayerVariables : public MMG_IStreamable
{
public:
	u32 myProfileId = 0;
	u32 myScore = 0;

	bool operator==(const MMG_TrackablePlayerVariables& aOther) const = default;

	void ToStream(MN_WriteMessage& theStream) const override;
	bool FromStream(MN_ReadMessage& theStream) override;
};
static_assert_size(MMG_TrackablePlayerVariables, 0xC);

// This container holds non-changing information set by the server at startup
class MMG_ServerStartupVariables : public MMG_IStreamable
{
public:
	MMG_GameNameString myServerName;
	u64	myCurrentMapHash = 0;
	MC_String myPublicIp;
	u32 myIp = 0;
	u32 myModId = 0;
	u16 myServerReliablePort = 0;
	u16 myMassgateCommPort = 0;
	u32 myGameVersion = 0;
	u16 myProtocolVersion = 0;
	u16 myMaxNumPlayers : 5 = 0;
	u16 myIsPasswordProtected : 1 = 0;
	u16 myVoipEnabled : 1 = 0;
	u16 myIsRanked : 1 = 0;
	u16 myIsDedicated : 1 = 0;
	ServerType myServerType = ServerType::NORMAL_SERVER;
	u32 myHostProfileId = 0; // This is needed for NAT Neg Servers, a value of 0 would indicate that no NAT Negging is possible 
	u32 myFingerprint = 0;
	u64 myServerId = 0; // Used for internal tracking in the Server Tracker. Should not be read from or written to streams.
	u8 myContainsPreorderMap = 0;
	bool myIsRankBalanced = false;
	bool myHasDominationMaps = false;
	bool myHasAssaultMaps = false;
	bool myHasTowMaps = false;

	const char* ServerTypeToString() const;

	void ToStream(MN_WriteMessage& theStream) const override;
	bool FromStream(MN_ReadMessage& theStream) override;
};
static_assert_size(MMG_ServerStartupVariables, 0xC8);

// This container holds information that can change as the users interact with the server
class MMG_TrackableServerFullInfo : public MMG_IStreamable
{
public:
	u64 myCurrentMapHash = 0;
	u64 myCycleHash = 0;
	MMG_GameNameString myServerName;
	u16 myServerReliablePort = 0;
	u32 myGameVersion = 0;
	u16 myProtocolVersion = 0;
	u32 myIp = 0;
	u32 myModId = 0;
	u16 myMassgateCommPort = 0;
	float myGameTime = 0.0f;
	u32 myPing = 9999;
	u32 myServerId = 0;
	u32 myCurrentLeader = 0;
	u32 myWinnerTeam = 0;
	u16 myNumPlayers : 5 = 0;
	u16 myMaxNumPlayers : 5 = 0;
	u16 myNumSpectators : 5 = 0; // Out of num players, this many are only spectating
	u16 myIsPasswordProtected : 1 = 1;
	u16 myIsRanked : 1 = 0;
	u16 myIsDedicated : 1 = 0;
	u16 myIsRankBalanced : 1 = 0;
	u16 myHasDominationMaps : 1 = 0;
	u16 myHasAssaultMaps : 1 = 0;
	u16 myHasTowMaps : 1 = 0;
	u32 myHostProfileId = 0;
	ServerType myServerType = ServerType::NORMAL_SERVER;
	MC_StaticArray<MMG_TrackablePlayerVariables, 64> myPlayers;

	bool operator==(const MMG_TrackableServerFullInfo& aRhs) const; // Does not compare gametime
	void SetType(const char* aType);

	void ToStream(MN_WriteMessage& theStream) const override;
	bool FromStream(MN_ReadMessage& theStream) override;
};
static_assert_size(MMG_TrackableServerFullInfo, 0x3D0);

class MMG_TrackableServerMinimalPingResponse
{
public:
	u64 myCurrentCycleHash = 0;
	u64 myCurrentMapHash = 0;
	u32 myIp = 0;
	u32 myModId = 0;
	u32 myPing = 0;
	u32 myServerId = 0;
	u16 myServerReliablePort = 0;
	u16 myIsPasswordProtected : 1 = 0;
	u16 myIsRanked : 1 = 0;
	u16 myIsDedicated : 1 = 0;
	u16 myIsRankBalanced : 1 = 0;
	u16 myHasDominationMaps : 1 = 0;
	u16 myHasAssaultMaps : 1 = 0;
	u16 myHasTowMaps : 1 = 0;

	MMG_GameNameString myGameName; // NOT TRANSFERED
	u32 myHostProfileId = 0; // NOT TRANSFERRED
	ServerType myServerType = ServerType::NORMAL_SERVER; // NOT TRANSFERRED
	u32 myMassgateCommPort = 0; // NOT TRANSFERRED

	u8 myNumPlayers = 0;
	u8 myMaxPlayers = 0;

	MMG_TrackableServerMinimalPingResponse() = default;
	MMG_TrackableServerMinimalPingResponse(const MMG_TrackableServerFullInfo& aRhs);
	MMG_TrackableServerMinimalPingResponse& operator=(const MMG_TrackableServerFullInfo& aRhs);

	void ToStream(MN_WriteMessage& theStream) const;
	bool FromStream(MN_ReadMessage& theStream);
};
static_assert_size(MMG_TrackableServerMinimalPingResponse, 0xB8);

// This container holds the heartbeat info sent by the ds to the master server list (i.e. from ds to massgate)
class MMG_TrackableServerHeartbeat : public MMG_IStreamable
{
public:
	u64 myCurrentMapHash = 0;
	float myGameTime = 0.0f;
	MC_StaticArray<u32, 64> myPlayersInGame;
	u32 myCurrentLeader = 0;
	u8 myMaxNumPlayers = 0;
	u8 myNumPlayers = 0;
	MMG_TrackableServerCookie myCookie;

	// Later - we may send info on the players that are on it
	// MMG_TrackablePlayerVariables myPlayers[xx];

	MMG_TrackableServerHeartbeat(const MMG_TrackableServerFullInfo& aRhs);
	bool operator==(const MMG_TrackableServerHeartbeat& aRhs) const;

	// Does not compare gametime
	bool IsAlike(const MMG_TrackableServerHeartbeat& aRhs) const;

	void ToStream(MN_WriteMessage& theStream) const override;
	bool FromStream(MN_ReadMessage& theStream) override;
};
static_assert_size(MMG_TrackableServerHeartbeat, 0x130);

// This container holds information about a server when requesting a serverlist (i.e. from Massgate to client)
class MMG_TrackableServerBriefInfo : public MMG_IStreamable
{
public:
	u64 myCycleHash = 0;
	u32 myIp = 0;
	u32 myModId = 0;
	u32 myServerId = 0;
	u16 myMassgateCommPort = 0;
	MMG_GameNameString myGameName;
	ServerType myServerType = ServerType::NORMAL_SERVER;
	bool myIsRankBalanced = false;

	MMG_TrackableServerBriefInfo(const MMG_TrackableServerFullInfo& aRhs);
	bool operator==(const MMG_TrackableServerBriefInfo& aRhs) const;

	void ToStream(MN_WriteMessage& theStream) const override;
	bool FromStream(MN_ReadMessage& theStream) override;
};
static_assert_size(MMG_TrackableServerBriefInfo, 0xA8);