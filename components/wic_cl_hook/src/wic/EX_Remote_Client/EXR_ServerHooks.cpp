#include <Hooks/Hooks.h>

void EXR_Server__ParseUnitFrameShooterChange_0077D481();

//
// Assert and kill the game if a unit has more than 8 active shooters. The server and client support
// units with more than 8 shooters, but the networking code doesn't. Else it'll cause memory corruption.
//
DECLARE_HOOK_INITIALIZER_MP(EXR_Server,
{
	Hooks::WriteJump(0x0077D481, &EXR_Server__ParseUnitFrameShooterChange_0077D481);
});

void TriggerAssert()
{
	MC_ASSERTMSG(false, "shooterIndex < WIC_MAX_NUM_SHOOTERS (8)");
}

void __declspec(naked) EXR_Server__ParseUnitFrameShooterChange_0077D481()
{
	__asm
	{
		// EDI is shooterIndex which indexes myShooterData[8]
		cmp edi, 8
		jb validArrayIndex

		call TriggerAssert
		int 3

		validArrayIndex:
		lea edx, dword ptr ds:[edi + edi * 2]
		lea ebp, dword ptr ds:[ebx + edx * 4]

		push 0x0077D487
		retn
	}
}