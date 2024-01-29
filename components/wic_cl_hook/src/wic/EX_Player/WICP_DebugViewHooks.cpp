#include <Hooks/Hooks.h>
#include "WICP_DebugView.h"

bool __fastcall hk_ReceiveChatPrivate(class EXP_Player *aThis, void *_EDX, const class EXCO_PlayerInfo *aPlayer, const wchar_t *aMessage);

//
// Register custom commands to show the ingame debug menu
//
DECLARE_HOOK_INITIALIZER_MP(WICP_DebugView,
{
	Hooks::WriteCall(0x00944A2E, &WICP_DebugView::Init);
	Hooks::WriteJump(0x00936680, &hk_ReceiveChatPrivate);
});