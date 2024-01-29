#include "EXCO_Map.h"

void EXCO_Map::Update()
{
#if WIC_MP
	const static u32 dwCall = 0x00979420;

	__asm
	{
		mov edi, this
		call [dwCall]
	}
#elif WIC_DS
	((void(__thiscall *)(EXCO_Map *))0x004419C0)(this);
#else
#error Unimplemented
#endif
}