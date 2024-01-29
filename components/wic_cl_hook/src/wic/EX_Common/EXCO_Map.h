#pragma once

#include <Hooks/Memory.h>

class EXCO_Map
{
public:
	static inline DeclareGlobalRef(EXCO_Map *, ourInstance, SelectOffset(0x00E186F4, 0x008EEF94));

	void Update();
};