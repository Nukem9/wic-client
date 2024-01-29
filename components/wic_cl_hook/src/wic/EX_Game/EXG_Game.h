#pragma once

#include "EXG_PlayerContainer.h"

class EXG_Game /* : public WICO_IGameInfo, public EXCO_IGame, public EXCO_Persistent<EXG_Game> */
{
public:
	char _padding[0x10030];
	EXG_PlayerContainer myPlayers;
};