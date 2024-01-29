#include <Hooks/Hooks.h>
#include "MC_SystemPaths.h"

//
// Cache results from SHGetFolderPathA. The game calls this several thousand times. It won't necessarily
// provide a performance boost, but it does clean up output in VTune and ProcMon.
//
DECLARE_HOOK_INITIALIZER_MP(MC_SystemPaths,
{
	Hooks::WriteJump(0x009F94C0, &MC_SystemPaths::GetUserDocumentsFileName);
});

DECLARE_HOOK_INITIALIZER_DS(MC_SystemPaths,
{
	Hooks::WriteJump(0x00425B00, &MC_SystemPaths::GetUserDocumentsFileName);
});