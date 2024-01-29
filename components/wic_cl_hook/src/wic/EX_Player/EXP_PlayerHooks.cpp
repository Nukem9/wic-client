#include <Hooks/Hooks.h>
#include "WICP_GameEventsHandler.h"
#include "WICP_Root.h"
#include "EXP_Player.h"

bool hk_call_0093BCDC();

//
// Display a HUD notification when an enemy nuke is launched
//
DECLARE_HOOK_INITIALIZER_MP(EXP_Player,
{
	Hooks::WriteJump(0x0093BCDC, &hk_call_0093BCDC);
});

bool __declspec(naked) hk_call_0093BCDC()
{
	const static u32 dwCall = 0x00BAEAE0;
	const static u32 dwReturn = 0x0093BCE1;

	bool detectedNuke;

	__asm
	{
		call [dwCall]

		pushad
		mov ebp, esp
		sub esp, __LOCAL_SIZE
		mov detectedNuke, al
	}

	if (detectedNuke && WICP_Root::ourPlayer->myGameEventsGui)
	{
		WICP_Root::ourPlayer->myGameEventsGui->AddScriptEvent(
			L"Enemy nuke launched.",
			"ui/minimap/minimap_ta_nuke.dds",
			0,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			false,
			WICP_GameEventsHandler::FUNC_NONE,
			0,
			nullptr);
	}

	__asm
	{
		mov esp, ebp
		popad
		jmp [dwReturn]
	}
}
