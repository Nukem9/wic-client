#pragma once

#include <MCommon2/MC_String.h>
#include <EX_Common/EXCO_Difficulty.h>
#include <EX_Common/WICO_BotOptions.h>

class WICO_GameSettings
{
public:
	enum Mode : s32
	{
		UNDEFINED = 0x0,
		SINGLE_PLAYER = 0x1,
		SINGLE_PLAYER_REPLAY = 0x2,
		SINGLE_PLAYER_CUSTOM = 0x3,
		SINGLE_PLAYER_BENCHMARK = 0x4,
		MULTI_PLAYER_HOST = 0x5,
		MULTI_PLAYER_HOST_COOP = 0x6,
		MULTI_PLAYER_JOIN = 0x7,
	};

	class MOS
	{
	public:
		u32 myMassgateProfileId;
		u32 myMassgateExperience;
		u32 myMassgateClanId;
		u8 myMassgateClanRank;
		bool myFireWallFlag;
		bool myReportFlag;
		MC_StaticString<16> myPublicIp;
		u16 myQueryPort;
		u32 myConnectCookie;
		u32 myNumSlotsRequired;
		u32 mySlotAllocationCookie;
		u32 myHostProfileId;
	};
	static_assert_size(MOS, 0x34);

	u32 myMaxPlayers;
	float myTimeLimitMultiplier;
	u32 myScoreLimit;
	u32 myAPFactor;
	u32 myStartingTickets;
	s32 myMaxNumAllowedTeamChanges;
	s32 myNumAllowedPlayersPerRole;
	Mode myMode;
	EXCO_Difficulty::Level mySinglePlayerDifficulty;
	bool myIsRankedGameFlag;
	bool myRandomizeMissionOrderFlag;
	u32 myAPRegrowthRate;
	MC_StaticString<16> myServerAddress;
	MC_StaticLocString<16> myPassword;
	MC_StaticLocString<16> myAdminPassword;
	MC_LocString myMessageOfTheDay;
	MC_StaticString<512> myBannerUrl;
	u16 myGamePortRangeStart;
	u16 myServerReliablePort;
	u16 myAdminPort;
	float myScriptedAPFactor;
	float myZoneTickAPFactor;
	float myUnitKilledAPFactor;
	float myUnitLostAPFactor;
	MC_StaticLocString<128> myGameName;
	MC_String myMapFilename;
	bool myPasswordedGameFlag;
	u32 myStartingAP[2];
	u32 myMaxAP[2];
	float myTimeLimitMultiplierOverride;
	bool myAllowSpectatorVotingFlag;
	bool myAllowSpectatingFlag;
	bool myTeamsAreLockedFlag;
	bool myLockedSpectatorTeams[2];
	bool myLockChatFromSpectatorsFlag;
	bool myAllowFreeSpectatingFlag;
	bool myGameOverFlag;
	bool myIsClanMatchFlag;
	bool myIsESLMatchFlag;
	bool myIsTournamentMatchFlag;
	bool myAutoBalanceTeamFlag;
	float myAutoBalanceStartDelay;
	WICO_BotOptions::BotMode myBotMode;
	bool myNonLinearDominationDisabled;
	bool myMatchModeFlag;
	bool myFPMModeFlag;
	bool myMatchStartReadyFlag;
	bool myStayOnTeamFlag;
	bool myRankBalanceTeams;
	float myMaxAllowedIdleTime;
	s32 myChatTimeSpan;
	s32 myMaxChatMessages;
	s32 myChatSpamMuteTime;
	s32 myRequestTimeSpan;
	s32 myMaxRequests;
	s32 myRequestSpamMuteTime;
	s32 myFriendlyFireLimit;
	bool myShowAdminTagFlag;
	s32 myMinimumPlayersRequired;
	s32 myModId;
	MOS myMOSConfig;
};
static_assert_size(WICO_GameSettings, 0x430);