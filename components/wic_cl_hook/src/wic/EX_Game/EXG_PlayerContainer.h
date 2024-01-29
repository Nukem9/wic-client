#pragma once

#include "../EX_Common/EXCO_Constants.h"

class EXCO_IPlayer;
class EXG_Player;

class EXG_PlayerContainer
{
public:
	char _padding[0x4C];
	EXG_Player *myPlayers[EX_MAX_NUM_CLIENTS];
	char _padding2[0x40];
	EXG_Player *myScriptGamePlayer;

public:
	EXCO_IPlayer *GetPlayerForSlot(u8 aSlot) const;
	EXG_Player *GetGamePlayerForSlot(u8 aSlot) const;
};
static_assert_offset(EXG_PlayerContainer, myPlayers, 0x4C);
static_assert_offset(EXG_PlayerContainer, myScriptGamePlayer, 0xCC);
//static_assert_size(EXG_PlayerContainer, 0xEC);