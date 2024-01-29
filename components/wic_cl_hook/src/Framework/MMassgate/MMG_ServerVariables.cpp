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
#include <MNetwork/MN_ReadMessage.h>
#include <MNetwork/MN_WriteMessage.h>
#include "MMG_Tiger.h"
#include "MMG_ServerVariables.h"

MMG_TrackableServerCookie::MMG_TrackableServerCookie() : trackid(0), hash(0)
{
}

bool MMG_TrackableServerCookie::operator==(const MMG_TrackableServerCookie& aOther) const
{
	return hash == aOther.hash && trackid == aOther.trackid;
}

bool
MMG_TrackableServerCookie::MatchesSecret(const s64 theSecret, const s64 thePreviousSecret, const MMG_Tiger& aHasher) const
{
	const u64 target = contents[1];

	u64 data[2] =
	{
		contents[0],
		contents[1]
	};

	// First try the current secret
	data[1] = theSecret;
	if (aHasher.GenerateHash(&data, sizeof(data)).Get64BitSubset() == target)
		return true;

	// No match, try the previous secret
	data[1] = thePreviousSecret;
	return aHasher.GenerateHash(&data, sizeof(data)).Get64BitSubset() == target;
}

void
MMG_TrackablePlayerVariables::ToStream(MN_WriteMessage& theStream) const
{
	theStream.WriteUInt(myProfileId);
	theStream.WriteUInt(myScore);
}

bool
MMG_TrackablePlayerVariables::FromStream(MN_ReadMessage& theStream)
{
	bool good = theStream.ReadUInt(myProfileId);
	good = good && theStream.ReadUInt(myScore);
	return good;
}

const char* MMG_ServerStartupVariables::ServerTypeToString() const
{
	// Do NOT rename return strings --- must be named exactly like this for db compatibility
	switch (myServerType)
	{
	case ServerType::NORMAL_SERVER:
		return "NORMAL";
	case ServerType::MATCH_SERVER:
		return "MATCH";
	case ServerType::FPM_SERVER:
		return "FPM";
	case ServerType::TOURNAMENT_SERVER:
		return "TOURNAMENT";
	case ServerType::CLANMATCH_SERVER:
		return "CLANMATCH";
	}

	MC_ASSERT(false);
	return nullptr;
}

void
MMG_ServerStartupVariables::ToStream(MN_WriteMessage& theStream) const
{
	const u32 randomValue = rand();

	theStream.WriteUInt(MMG_Protocols::MassgateProtocolVersion);
	theStream.WriteUInt(myGameVersion);
	theStream.WriteUShort(myProtocolVersion);
	theStream.WriteUInt(randomValue);
	theStream.WriteUInt(myFingerprint ^ randomValue);
	theStream.WriteUInt(myIp);
	theStream.WriteUInt(myModId);
	theStream.WriteUShort(myServerReliablePort);
	theStream.WriteUShort(myMassgateCommPort);
	theStream.WriteLocString(myServerName.GetBuffer(), myServerName.GetLength());
	theStream.WriteUChar(myMaxNumPlayers);
	theStream.WriteUChar(myIsPasswordProtected);
	theStream.WriteUChar(myIsDedicated);
	theStream.WriteUChar(myVoipEnabled);
	theStream.WriteUChar(myIsRanked);
	theStream.WriteUChar(static_cast<u8>(myServerType));
	theStream.WriteUInt64(myCurrentMapHash);
	theStream.WriteString(myPublicIp);
	theStream.WriteUInt(myHostProfileId);
	theStream.WriteUChar(myContainsPreorderMap);
	theStream.WriteBool(myIsRankBalanced);
	theStream.WriteBool(myHasDominationMaps);
	theStream.WriteBool(myHasAssaultMaps);
	theStream.WriteBool(myHasTowMaps);
}

bool
MMG_ServerStartupVariables::FromStream(MN_ReadMessage& theStream)
{
	bool good = true;
	u32 protocolVersion = 0;

	good = good && theStream.ReadUInt(protocolVersion);

	if (good && (protocolVersion != MMG_Protocols::MassgateProtocolVersion))
	{
		MC_DEBUG("Invalid protocol version");
		return false;
	}

	u32 randomValue = 0;
	u32 temp = 0;
	u8 tempChar = 0;

	good = good && theStream.ReadUInt(myGameVersion);
	good = good && theStream.ReadUShort(myProtocolVersion);
	good = good && theStream.ReadUInt(randomValue);
	good = good && theStream.ReadUInt(temp);
	good = good && theStream.ReadUInt(myIp);
	good = good && theStream.ReadUInt(myModId);
	good = good && theStream.ReadUShort(myServerReliablePort);
	good = good && theStream.ReadUShort(myMassgateCommPort);
	good = good && theStream.ReadLocString(myServerName.GetBuffer(), myServerName.GetBufferSize());

	good = good && theStream.ReadUChar(tempChar); myMaxNumPlayers = tempChar;
	good = good && theStream.ReadUChar(tempChar); myIsPasswordProtected = tempChar;
	good = good && theStream.ReadUChar(tempChar); myIsDedicated = tempChar;
	good = good && theStream.ReadUChar(tempChar); myVoipEnabled = tempChar;
	good = good && theStream.ReadUChar(tempChar); myIsRanked = tempChar;
	good = good && theStream.ReadUChar(tempChar); myServerType = static_cast<ServerType>(tempChar);

	good = good && theStream.ReadUInt64(myCurrentMapHash);
	good = good && theStream.ReadString(myPublicIp);
	good = good && theStream.ReadUInt(myHostProfileId);
	good = good && theStream.ReadUChar(myContainsPreorderMap);

	good = good && theStream.ReadBool(myIsRankBalanced);
	good = good && theStream.ReadBool(myHasDominationMaps);
	good = good && theStream.ReadBool(myHasAssaultMaps);
	good = good && theStream.ReadBool(myHasTowMaps);

	if (good)
		myFingerprint = temp ^ randomValue;

	return good;
}

bool
MMG_TrackableServerFullInfo::operator==(const MMG_TrackableServerFullInfo& aRhs) const
{
	// DOES NOT COMPARE GAMETIME OR PING
	// check server

	bool same = (
		(myCycleHash == aRhs.myCycleHash) &&
		(myCurrentMapHash == aRhs.myCurrentMapHash) &&
		(myServerName == aRhs.myServerName) &&
		(myServerReliablePort == aRhs.myServerReliablePort) &&
		(myGameVersion == aRhs.myGameVersion) &&
		(myProtocolVersion == aRhs.myProtocolVersion) &&
		(myIsPasswordProtected == aRhs.myIsPasswordProtected) &&
		(myIsRanked == aRhs.myIsRanked) &&
		(myIsRankBalanced == aRhs.myIsRankBalanced) &&
		(myHasDominationMaps == aRhs.myHasDominationMaps) &&
		(myHasAssaultMaps == aRhs.myHasAssaultMaps) &&
		(myHasTowMaps == aRhs.myHasTowMaps) &&
		(myServerType == aRhs.myServerType) &&
		(myIsDedicated == aRhs.myIsDedicated) &&
		(myIp == aRhs.myIp) &&
		(myModId == aRhs.myModId) &&
		(myMassgateCommPort == aRhs.myMassgateCommPort) &&
		(myMaxNumPlayers == aRhs.myMaxNumPlayers) &&
		(myNumSpectators == aRhs.myNumSpectators) &&
		(myNumPlayers == aRhs.myNumPlayers) &&
		(myServerId == aRhs.myServerId) &&
		(myCurrentLeader == aRhs.myCurrentLeader) &&
		(myWinnerTeam == aRhs.myWinnerTeam) &&
		(myHostProfileId == aRhs.myHostProfileId));

	for (u32 i = 0; same && (i < myNumPlayers); i++)
		same = same && (myPlayers[i] == aRhs.myPlayers[i]);

	return same;
}

void
MMG_TrackableServerFullInfo::SetType(const char* aType)
{
	MC_StaticString<32> t = aType;

	// Do NOT rename strings --- must be named exactly like this for db compatibility
	if (t == "NORMAL")
		myServerType = ServerType::NORMAL_SERVER;
	else if (t == "FPM")
		myServerType = ServerType::FPM_SERVER;
	else if (t == "MATCH")
		myServerType = ServerType::MATCH_SERVER;
	else if (t == "TOURNAMENT")
		myServerType = ServerType::TOURNAMENT_SERVER;
	else if (t == "CLANMATCH")
		myServerType = ServerType::CLANMATCH_SERVER;
	else
		MC_ASSERTMSG(false, "Invalid server type.");
}

void
MMG_TrackableServerFullInfo::ToStream(MN_WriteMessage& theStream) const
{
	theStream.WriteUInt(myGameVersion);
	theStream.WriteUShort(myProtocolVersion);
	theStream.WriteUInt64(myCurrentMapHash);
	theStream.WriteUInt64(myCycleHash);
	theStream.WriteLocString(myServerName.GetBuffer(), myServerName.GetLength());
	theStream.WriteUShort(myServerReliablePort);

	theStream.WriteUChar(myMaxNumPlayers);
	theStream.WriteUChar(myNumPlayers);
	theStream.WriteUChar(myIsPasswordProtected);
	theStream.WriteUChar(myIsDedicated);
	theStream.WriteUChar(myIsRanked);
	theStream.WriteUChar(myIsRankBalanced);
	theStream.WriteUChar(myHasDominationMaps);
	theStream.WriteUChar(myHasAssaultMaps);
	theStream.WriteUChar(myHasTowMaps);
	theStream.WriteUChar(static_cast<u8>(myServerType));

	theStream.WriteUInt(myIp);
	theStream.WriteUInt(myModId);
	theStream.WriteUShort(myMassgateCommPort);
	theStream.WriteFloat(myGameTime);
	theStream.WriteUInt(myServerId);
	theStream.WriteUInt(myCurrentLeader);
	theStream.WriteUInt(myHostProfileId);
	theStream.WriteUInt(myWinnerTeam);

	for (u32 i = 0; i < myNumPlayers; i++)
		myPlayers[i].ToStream(theStream);
}

bool
MMG_TrackableServerFullInfo::FromStream(MN_ReadMessage& theStream)
{
	bool good = true;
	u8 tempChar = 0;

	good = good && theStream.ReadUInt(myGameVersion);
	good = good && theStream.ReadUShort(myProtocolVersion);
	good = good && theStream.ReadUInt64(myCurrentMapHash);
	good = good && theStream.ReadUInt64(myCycleHash);
	good = good && theStream.ReadLocString(myServerName.GetBuffer(), myServerName.GetBufferSize());
	good = good && theStream.ReadUShort(myServerReliablePort);

	good = good && theStream.ReadUChar(tempChar); myMaxNumPlayers = tempChar;
	good = good && theStream.ReadUChar(tempChar); myNumPlayers = tempChar;
	good = good && theStream.ReadUChar(tempChar); myIsPasswordProtected = tempChar;
	good = good && theStream.ReadUChar(tempChar); myIsDedicated = tempChar;
	good = good && theStream.ReadUChar(tempChar); myIsRanked = tempChar;
	good = good && theStream.ReadUChar(tempChar); myIsRankBalanced = tempChar;
	good = good && theStream.ReadUChar(tempChar); myHasDominationMaps = tempChar;
	good = good && theStream.ReadUChar(tempChar); myHasAssaultMaps = tempChar;
	good = good && theStream.ReadUChar(tempChar); myHasTowMaps = tempChar;
	good = good && theStream.ReadUChar(tempChar); myServerType = static_cast<ServerType>(tempChar);

	good = good && theStream.ReadUInt(myIp);
	good = good && theStream.ReadUInt(myModId);
	good = good && theStream.ReadUShort(myMassgateCommPort);
	good = good && theStream.ReadFloat(myGameTime);
	good = good && theStream.ReadUInt(myServerId);
	good = good && theStream.ReadUInt(myCurrentLeader);
	good = good && theStream.ReadUInt(myHostProfileId);
	good = good && theStream.ReadUInt(myWinnerTeam);

	// We didn't write ping, made no sense
	for (u32 i = 0; good && (i < myNumPlayers); i++)
		good = good && myPlayers[i].FromStream(theStream);

	return good;
}

MMG_TrackableServerMinimalPingResponse::MMG_TrackableServerMinimalPingResponse(const MMG_TrackableServerFullInfo& aRhs)
{
	*this = aRhs;
}

MMG_TrackableServerMinimalPingResponse&
MMG_TrackableServerMinimalPingResponse::operator=(const MMG_TrackableServerFullInfo& aRhs)
{
	myCurrentMapHash = aRhs.myCurrentMapHash;
	myCurrentCycleHash = aRhs.myCycleHash;
	myServerReliablePort = aRhs.myServerReliablePort;
	myIp = aRhs.myIp;
	myServerId = aRhs.myServerId;
	myModId = aRhs.myModId;
	myIsPasswordProtected = aRhs.myIsPasswordProtected;
	myIsRanked = aRhs.myIsRanked;
	myIsDedicated = aRhs.myIsDedicated;
	myIsRankBalanced = aRhs.myIsRankBalanced;
	myHasDominationMaps = aRhs.myHasDominationMaps;
	myHasAssaultMaps = aRhs.myHasAssaultMaps;
	myHasTowMaps = aRhs.myHasTowMaps;
	myNumPlayers = aRhs.myNumPlayers;
	myMaxPlayers = aRhs.myMaxNumPlayers;
	myServerType = aRhs.myServerType;
	myHostProfileId = aRhs.myHostProfileId;
	myGameName = aRhs.myServerName;
	myMassgateCommPort = aRhs.myMassgateCommPort;
	return *this;
}

void
MMG_TrackableServerMinimalPingResponse::ToStream(MN_WriteMessage& theStream) const
{
	const auto tempFlags = static_cast<u8>((myHasTowMaps << 6) |
		(myHasAssaultMaps << 5) |
		(myHasDominationMaps << 4) |
		(myIsRankBalanced << 3) |
		(myIsPasswordProtected << 2) |
		(myIsRanked << 1) |
		myIsDedicated);

	theStream.WriteUInt64(myCurrentMapHash);
	theStream.WriteUInt64(myCurrentCycleHash);
	theStream.WriteUShort(myServerReliablePort);
	theStream.WriteUInt(myIp);
	theStream.WriteUInt(myServerId);
	theStream.WriteUInt(myModId);
	theStream.WriteUChar(tempFlags);
	theStream.WriteUChar(myNumPlayers);
	theStream.WriteUChar(myMaxPlayers);
}

bool
MMG_TrackableServerMinimalPingResponse::FromStream(MN_ReadMessage& theStream)
{
	bool good = true;
	u8 tmp = 0;

	good = good && theStream.ReadUInt64(myCurrentMapHash);
	good = good && theStream.ReadUInt64(myCurrentCycleHash);
	good = good && theStream.ReadUShort(myServerReliablePort);
	good = good && theStream.ReadUInt(myIp);
	good = good && theStream.ReadUInt(myServerId);
	good = good && theStream.ReadUInt(myModId);
	good = good && theStream.ReadUChar(tmp);
	good = good && theStream.ReadUChar(myNumPlayers);
	good = good && theStream.ReadUChar(myMaxPlayers);

	myHasTowMaps = (tmp & (1 << 6)) > 0;
	myHasAssaultMaps = (tmp & (1 << 5)) > 0;
	myHasDominationMaps = (tmp & (1 << 4)) > 0;
	myIsRankBalanced = (tmp & (1 << 3)) > 0;
	myIsPasswordProtected = (tmp & (1 << 2)) > 0;
	myIsRanked = (tmp & (1 << 1)) > 0;
	myIsDedicated = (tmp & (1)) > 0;
	return good;
}

MMG_TrackableServerHeartbeat::MMG_TrackableServerHeartbeat(const MMG_TrackableServerFullInfo& aRhs)
	: myCurrentMapHash(aRhs.myCurrentMapHash)
	, myMaxNumPlayers(aRhs.myMaxNumPlayers)
	, myNumPlayers(aRhs.myNumPlayers)
	, myGameTime(aRhs.myGameTime)
	, myCurrentLeader(aRhs.myCurrentLeader)
{
	for (u32 i = 0; i < myNumPlayers; i++)
		myPlayersInGame[i] = aRhs.myPlayers[i].myProfileId;
}

bool
MMG_TrackableServerHeartbeat::operator==(const MMG_TrackableServerHeartbeat& aRhs) const
{
	return myCookie == aRhs.myCookie;
}

bool MMG_TrackableServerHeartbeat::IsAlike(const MMG_TrackableServerHeartbeat& aRhs) const
{
	return
		myCurrentMapHash == aRhs.myCurrentMapHash &&
		myCurrentLeader == aRhs.myCurrentLeader &&
		myMaxNumPlayers == aRhs.myMaxNumPlayers &&
		myNumPlayers == aRhs.myNumPlayers;
}

void
MMG_TrackableServerHeartbeat::ToStream(MN_WriteMessage& theStream) const
{
	theStream.WriteUInt64(myCurrentMapHash);
	theStream.WriteUInt(myCurrentLeader);
	theStream.WriteUChar(myMaxNumPlayers);
	theStream.WriteUChar(myNumPlayers);
	theStream.WriteFloat(myGameTime);
	theStream.WriteRawData(&myCookie, sizeof(myCookie));

	for (u32 i = 0; i < myNumPlayers; i++)
		theStream.WriteUInt(myPlayersInGame[i]);
}

bool
MMG_TrackableServerHeartbeat::FromStream(MN_ReadMessage& theStream)
{
	bool good = true;
	good = good && theStream.ReadUInt64(myCurrentMapHash);
	good = good && theStream.ReadUInt(myCurrentLeader);
	good = good && theStream.ReadUChar(myMaxNumPlayers);
	good = good && theStream.ReadUChar(myNumPlayers);
	good = good && theStream.ReadFloat(myGameTime);
	good = good && theStream.ReadRawData(&myCookie, sizeof(myCookie));

	for (u32 i = 0; good && (i < myNumPlayers); i++)
		good = good && theStream.ReadUInt(myPlayersInGame[i]);

	return good;
}

MMG_TrackableServerBriefInfo::MMG_TrackableServerBriefInfo(const MMG_TrackableServerFullInfo& aRhs)
	: myIp(aRhs.myIp)
	, myModId(aRhs.myModId)
	, myMassgateCommPort(aRhs.myMassgateCommPort)
	, myCycleHash(aRhs.myCycleHash)
	, myGameName(aRhs.myServerName)
	, myServerId(aRhs.myServerId)
	, myServerType(aRhs.myServerType)
{
}

bool
MMG_TrackableServerBriefInfo::operator==(const MMG_TrackableServerBriefInfo& aRhs) const
{
	return	myServerId == aRhs.myServerId;
}

void
MMG_TrackableServerBriefInfo::ToStream(MN_WriteMessage& theStream) const
{
	theStream.WriteLocString(myGameName.GetBuffer(), myGameName.GetLength());
	theStream.WriteUInt(myIp);
	theStream.WriteUInt(myModId);
	theStream.WriteUInt(myServerId);
	theStream.WriteUShort(myMassgateCommPort);
	theStream.WriteUInt64(myCycleHash);
	theStream.WriteUChar(static_cast<u8>(myServerType));
	theStream.WriteBool(myIsRankBalanced);
}

bool
MMG_TrackableServerBriefInfo::FromStream(MN_ReadMessage& theStream)
{
	bool good = true;
	u8 tmpChar = 0;

	good = good && theStream.ReadLocString(myGameName.GetBuffer(), myGameName.GetBufferSize());
	good = good && theStream.ReadUInt(myIp);
	good = good && theStream.ReadUInt(myModId);
	good = good && theStream.ReadUInt(myServerId);
	good = good && theStream.ReadUShort(myMassgateCommPort);
	good = good && theStream.ReadUInt64(myCycleHash);
	good = good && theStream.ReadUChar(tmpChar); myServerType = static_cast<ServerType>(tmpChar);
	good = good && theStream.ReadBool(myIsRankBalanced);
	return good;
}