#include "../stdafx.h"

DPTR(MC_CommandLine, ourInstance);

bool MC_CommandLine::IsPresent(const char *Option)
{
	__asm
	{
		mov eax, this
		push Option
		mov ecx, 0x009FA4C0
		call ecx
	}
}