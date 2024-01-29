#pragma once

#include <MCommon2/MC_StaticArray.h>
#include "EXCO_PlayerInfo.h"
#include "EXCO_Constants.h"

class WICO_GameSettings;

class EXCO_MissionInfo
{
public:
	MC_StaticArray<EXCO_PlayerInfo, EX_MAX_NUM_PLAYERS> myPlayers;
	EXCO_PlayerInfo myScriptPlayerInfo;
	WICO_GameSettings& myGameSettings;
	char _pad[0x37C];
};
static_assert_offset(EXCO_MissionInfo, myScriptPlayerInfo, 0x2700);
static_assert_size(EXCO_MissionInfo, 0x2CF0);