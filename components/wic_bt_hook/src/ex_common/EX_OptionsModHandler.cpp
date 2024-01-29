#include "../stdafx.h"

void EX_OptionsModHandler::InitializeHook()
{
	Detours::X86::DetourFunctionT(0x00B5A340, &HandleEvent__SetActiveMod_00B5A340, Detours::X86Option::USE_CALL);
	Detours::X86::DetourFunctionT(0x00B5A399, &HandleEvent__SetActiveMod_00B5A340, Detours::X86Option::USE_CALL);
}

void __declspec(naked) EX_OptionsModHandler::HandleEvent__SetActiveMod_00B5A340()
{
	__asm
	{
		// eax contains selected mod id
		// ecx contains EX_App::ourInstance
		mov ecx, dword ptr ds:[0x00E30FA8]
		mov dword ptr ds:[ecx + 0x10F0], eax

		// EX_AppStateNavigator::SetEvent(EVENT_SET_ACTIVE_MOD)
		mov eax, 0xD
		lea edx, dword ptr ds:[ecx + 0x10EC]
		mov ecx, 0x00B4D5A0
		call ecx

		mov al, 0x1
		retn 0x8
	}
}