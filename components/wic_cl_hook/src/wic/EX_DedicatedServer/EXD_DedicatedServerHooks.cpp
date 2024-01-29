#include <Hooks/Hooks.h>
#include "EXD_DedicatedServer.h"

//
// Hook the main tick update function to allow us to run our own code
//
DECLARE_HOOK_INITIALIZER_DS(EXD_DedicatedServer,
{
	Hooks::WriteJump(0x004057C0, &EXD_DedicatedServer::Update);
});