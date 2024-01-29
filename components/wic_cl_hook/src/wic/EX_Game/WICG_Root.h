#pragma once

#include <Hooks/Memory.h>

class EXG_Game;

class WICG_Root
{
public:
	static inline DeclareGlobalRef(EXG_Game *, ourGame, SelectOffset(0x00E30D70, 0x008F3378));
};