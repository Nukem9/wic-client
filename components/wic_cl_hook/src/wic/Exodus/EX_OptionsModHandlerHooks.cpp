#include <Hooks/Hooks.h>

void hk_EX_OptionsModHandler__HandleEvent__SetActiveMod_00B4FC10();

//
// Fix a bug where mods were loaded synchronously in a GUI handler when they should've been using the
// app event queue. MG_Gui::UpdateInternal or MG_Gui::Update would crash after 'MG_Gui *this' was
// deleted INSIDE its own event handler.
//
DECLARE_HOOK_INITIALIZER_MP(EX_OptionsModHandler,
{
	Hooks::WriteCall(0x00B4FC10, &hk_EX_OptionsModHandler__HandleEvent__SetActiveMod_00B4FC10);
	Hooks::WriteCall(0x00B4FC69, &hk_EX_OptionsModHandler__HandleEvent__SetActiveMod_00B4FC10);
});

void __declspec(naked) hk_EX_OptionsModHandler__HandleEvent__SetActiveMod_00B4FC10()
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