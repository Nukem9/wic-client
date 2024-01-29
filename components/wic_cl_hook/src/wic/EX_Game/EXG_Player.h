#pragma once

class EXCO_IPlayer;

class EXG_Player
{
public:
	char _padding[0x1C];
	EXCO_IPlayer& myPlayer;
	char _padding2[0xF2];
	bool myPlayerDeletedFlag;
};
static_assert_offset(EXG_Player, _padding2, 0x1C + 0x4);
static_assert_offset(EXG_Player, myPlayerDeletedFlag, 0x112);