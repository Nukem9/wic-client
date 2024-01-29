#include <Hooks/Hooks.h>
#include "WICP_MegaMap.h"
#include "WICP_Root.h"
#include "EXP_UIHandler.h"

void __stdcall hk_call_0090E447(WICP_MegaMap *aThis, bool aFlyToDZFlag, bool aIsCancelledFlag);

//
// Disable fly to drop zone when we have set a DZ previously. The final camera location is exactly
// where the player started.
//
DECLARE_HOOK_INITIALIZER_MP(WICP_MegaMap,
{
	Hooks::WriteCall(0x0090E447, &hk_call_0090E447);
});

void __stdcall hk_call_0090E447(WICP_MegaMap *aThis, bool aFlyToDZFlag, bool aIsCancelledFlag)
{
	if (WICP_Root::ourUIHandler->myHasSetDZFlag)
		aFlyToDZFlag = false;

	aThis->EndDZPlacing(aFlyToDZFlag, aIsCancelledFlag);
}