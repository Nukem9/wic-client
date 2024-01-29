#pragma once

#include <MCommon2/MC_String.h>
#include <EX_Common/EXCO_MissionInfo.h>
#include <EX_Common/EXCO_MissionSet.h>
#include <EX_Common/EXCO_SlotManager.h>
#include <EX_Common/WICO_GameSettings.h>
#include <EX_Game/WICG_VoipServer.h>
#include <EX_Remote/WICR_GameController.h>

class Ice;
class WICR_ClientFactory;

class EXD_DedicatedServer
{
public:
	EXCO_MissionInfo myMission;
	WICR_ClientFactory *myClientFactory;
	WICR_GameController *myGameController;
	EXCO_SlotManager mySlotManager;
	EXCO_MissionSet myMissions;
	EXCO_MissionSet myMapCycle;
	const Ice *myGameText;
	bool myReportedStatsFlag;
	s32 myRestartInterval;
	s32 myNumGames;
	MC_LocString myGamename;
	MC_LocString myModName;
	WICR_GameController::WICR_GameInitData myIniData;
	float myNextDebugTime;
	u32 myNumFrames;
	WICO_GameSettings myGameSettings;
	u32 myTempBuffer[120];
	WICG_VoipServer myVoipServer;
	float myGracefulShutdownTime;
	float myNextWarningTime;
	bool myHaveReportedShutdownFlag;

	bool Update();
	void UpdateShutdownState();
	void PrintStats();
};
static_assert_size(EXD_DedicatedServer, 0x379C);