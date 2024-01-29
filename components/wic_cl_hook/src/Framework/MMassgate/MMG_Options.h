#pragma once

#include <Hooks/Memory.h>
#include <MCommon2/MC_HybridArray.h>
#include "MMG_Constants.h"

class MMG_Options
{
private:
	static inline DeclareGlobalRef(MMG_Options *, ourInstance, SelectOffset(0x00E30D3C, 0x008F3254));

public:
	static MMG_Options* GetInstance();

	MC_HybridArray<u64, 64> myHatedMaps;
	bool myPccInGame;
	bool myShowMyPcc;
	bool myShowFriendsPcc;
	bool myShowOthersPcc;
	bool myShowClanPccInGame;
	bool myGameModeAny;
	bool myGameModeDomination;
	bool myGameModeAssault;
	bool myGameModeTow;
	u8 myRankBalancePref; // 0 = don't care, 1 = yes, 2 = no
	float myOpponnentPreference;

	bool myDoNatNegotiationFlag;
	u32 myDownloadWhen;
	u32 myInternetConnection;

	MMG_GameNameString myLastServerPlayed;

	s32 myReceiveFromFriends;
	s32 myReceiveFromClanMembers;
	s32 myReceiveFromAcquaintances;
	s32 myReceiveFromEveryoneElse;
	s32 myAllowCommunicationInGame;
	s32 myEmailIfOffline;
	s32 myGroupInviteFromFriendsOnly;
	s32 myClanInviteFromFriendsOnly;
	s32 myServerInviteFromFriendsOnly;

	s32 myPingsPerSecond;
};
static_assert_size(MMG_Options, 0x2D8);