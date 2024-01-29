#pragma once

#include <Hooks/Memory.h>

class EXCO_IGame;
class EXP_Player;
class EXP_UIHandler;

class WICP_Root
{
public:
	static inline DeclareGlobalRef(EXP_Player *, ourPlayer, 0x00E30B00);
	static inline DeclareGlobalRef(EXCO_IGame *, ourGameHandler, 0x00E30B04);
	static inline DeclareGlobalRef(EXP_UIHandler *, ourUIHandler, 0x00E30B08);
};