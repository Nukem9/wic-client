#include "../stdafx.h"

void EX_CAI_Platoon::InitializeHook()
{
	Detours::X86::DetourFunctionClass(reinterpret_cast<uint8_t *>(0x00807D34), &AssesSituation_00807D34, Detours::X86Option::USE_CALL);
}

void __declspec(naked) EX_CAI_Platoon::AssesSituation_00807D34()
{
	__asm
	{
		mov eax, 0x00A02800
		call eax
		add esp, 0x10

		push 0x00807D6B
		retn
	}
}