#pragma once

#include "WICO_Team.h"

class WICO_GameSettings
{
public:
	unsigned int GetStartingAp(WICO_Team::AllowedTeam anAllowedTeam)
	{
		return ((unsigned int(__thiscall *)(WICO_GameSettings *, WICO_Team::AllowedTeam))0x0044B860)(this, anAllowedTeam);
	}

	unsigned int GetMaxAp(WICO_Team::AllowedTeam anAllowedTeam)
	{
		return ((unsigned int(__thiscall *)(WICO_GameSettings *, WICO_Team::AllowedTeam))0x0044B880)(this, anAllowedTeam);
	}
};