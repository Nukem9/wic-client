#pragma once

#include <MCommon2/MC_String.h>
#include <EX_Common/WICO_BotOptions.h>

class EXCO_DamageModel1;
class EXCO_UnitTypeSet;
class EXG_Game;
class EXR_LANGameFinderServer;
class EXR_MOSGameFinderServer;
class WICO_BuildingTypeSet;
class WICO_GameSettings;
class WICO_GlobalUnitValues;
class EXCO_MissionInfo;
class EXCO_MissionSet;
class WICO_PropTypeSet;
class WICR_ClientFactory;

class WICR_GameController
{
public:
	class WICR_GameInitData
	{
	public:
		MC_String myMapCycleFile;
		MC_String myPublicIP;
		MC_String myCustomMapPath;
		MC_LocString myGameName;
		MC_LocString myPassword;
		MC_LocString myAdminPassword;
		MC_LocString myMessageOfTheDay;
		MC_StaticString<512> myBannerUrl;
		s32 myMaxPlayers;
		s32 myMaxNumAllowedTeamChanges;
		s32 myNumAllowedPlayersPerRole;
		s32 myMaxAllowedIdleTime;
		s32 myChatTimeSpan;
		s32 myMaxChatMessages;
		s32 myChatSpamMuteTime;
		s32 myRequestTimeSpan;
		s32 myMaxRequests;
		s32 myRequestSpamMuteTime;
		s32 myFriendlyFireLimit;
		bool myShowAdminTagFlag;
		s32 myMinimumPlayersRequired;
		s32 myBanTime;
		MC_String myBanFileName;
		float myTimeLimitMultiplier;
		u16 myMassgateQueryPort;
		u16 myGamePortRangeStart;
		u16 myAdminPort;
		u32 myTournamentClanA;
		u32 myTournamentClanB;
		bool myTournamentServerFlag;
		bool myClanMatchServerFlag;
		bool myIsESLMatchFlag;
		bool myReportToMassgateFlag;
		bool myRankedGameFlag;
		bool myUseFireWallSettingsFlag;
		bool myIsMatchMode;
		bool myIsFPMMode;
		bool myNonLinearDominationDisabled;
		s32 myModId;
		bool myRandomizeMissionOrderFlag;
		bool myUseAutoTeamBalanceFlag;
		float myAutoBalanceStartDelay;
		bool myAllowSpectatorVotingFlag;
		bool myAllowSpectatingFlag;
		bool myAllowFreeSpectatingFlag;
		bool myStayOnTeamFlag;
		bool myRankBalanceTeams;
		u32 mySupplierId;
		WICO_BotOptions myBotOptions;
	};
	static_assert_size(WICR_GameInitData, 0x3C4);

	char _pad0[0xC];
	EXG_Game *myGame;
	WICR_ClientFactory *myClientFactory;
	char _pad1[0x68];
	//EXCO_Campaign myCampaign;
	//EXCO_CloudTypeSet myCloudTypes;
	//WICO_DeployableTypeSet myDeployableTypes;
	EXCO_DamageModel1 *myDamageModel;
	WICO_GlobalUnitValues *myGlobalUnitValues;
	EXCO_UnitTypeSet *myUnitTypes;
	WICO_BuildingTypeSet *myBuildingTypes;
	WICO_PropTypeSet *myPropTypes;
	EXR_LANGameFinderServer *myLANGameFinderServer;
	EXR_MOSGameFinderServer *myMOSGameFinderServer;
	WICR_GameController::WICR_GameInitData *myGameInitData;
	WICO_GameSettings& myGameSettings;
	EXCO_MissionSet& myMissions;
	EXCO_MissionInfo& myMission;

	bool Update();
	bool UpdateMassgate();
};
static_assert_offset(WICR_GameController, myGameInitData, 0x98);