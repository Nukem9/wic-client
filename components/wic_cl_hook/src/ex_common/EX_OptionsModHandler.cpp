#include "../stdafx.h"

void EX_OptionsModHandler::InitializeHook()
{
	Detours::X86::DetourFunctionClass(reinterpret_cast<uint8_t *>(0x00B4FC10), &HandleEvent__SetActiveMod_00B4FC10, Detours::X86Option::USE_CALL);
	Detours::X86::DetourFunctionClass(reinterpret_cast<uint8_t *>(0x00B4FC69), &HandleEvent__SetActiveMod_00B4FC10, Detours::X86Option::USE_CALL);
}

void __declspec(naked) EX_OptionsModHandler::HandleEvent__SetActiveMod_00B4FC10()
{
	__asm
	{
		// eax contains selected mod id
		// ecx contains EX_App::ourInstance
		mov ecx, dword ptr ds:[0x00E1E664]
		mov dword ptr ds:[ecx + 0x10F0], eax

		// EX_AppStateNavigator::SetEvent(EVENT_SET_ACTIVE_MOD)
		mov eax, 0xD
		lea edx, dword ptr ds:[ecx + 0x10EC]
		mov ecx, 0x00B42D00
		call ecx

		mov al, 0x1
		retn 0x8
	}
}