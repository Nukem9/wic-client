#include <Hooks/Hooks.h>
#include "MMG_ServerTracker.h"

void __stdcall hk_00791D40(MMG_ServerTracker *aThis);

//
//
//
DECLARE_HOOK_INITIALIZER_MP(MMG_ServerTracker,
{
	Hooks::WriteJump(0x00791D40, &hk_00791D40);
});

void __stdcall hk_00791D40(MMG_ServerTracker *aThis)
{
	return aThis->PrivHandlePingers();
}