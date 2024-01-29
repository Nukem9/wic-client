#pragma once

#include "../EX_Common/EXCO_IPlayer.h"

class MG_Manager;
class WICP_GameEventsHandler;

class EXP_Player : public EXCO_IPlayer
{
public:
	char _pad0[0x10758];
	MG_Manager& myGuiManager;
	char _pad1[0x8DC];
	WICP_GameEventsHandler *myGameEventsGui;
};
static_assert_offset(EXP_Player, _pad1, 0x10780 + 0x4);
//static_assert_offset(EXP_Player, myGuiManager, 0x10780);
static_assert_offset(EXP_Player, myGameEventsGui, 0x11060);