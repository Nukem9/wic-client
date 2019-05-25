#pragma once

#include "WICO_GameSettings.h"

class EXCO_MissionInfo
{
	char _pad[0x29B4];

public:
	WICO_GameSettings *myGameSettings;
};