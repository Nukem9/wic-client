#include <Hooks/Hooks.h>

void hk_0050D9B4();

//
// Fix a rare crash in EXG_Shooter::ParasiteUpdate(). This occurs when a unit is commanded to force fire
// on an agent (e.g. a building or squad) while reacquiring a target that is no longer valid (dead).
// 
// TL;DR: WICG_MultiAgentShooterTarget ends up with zero targets, which trips an assertion when calling
// GetPosition() later down the line.
//
DECLARE_HOOK_INITIALIZER_MP(EXG_Shooter,
{
	Hooks::WriteJump(0x006FEA9E, &hk_0050D9B4);
});

DECLARE_HOOK_INITIALIZER_DS(EXG_Shooter,
{
	Hooks::WriteJump(0x0050D9B4, &hk_0050D9B4);
});

void __declspec(naked) hk_0050D9B4()
{
	const static u32 dwIsInvalidTargetJump = SelectOffset(0x006FEC55, 0x0050DB6D);
	const static u32 dwIsValidTargetJump = SelectOffset(0x006FEAA4, 0x0050D9BA);

	__asm
	{
		jz InvalidTarget
		push ecx

		// Call EXG_ShooterTarget::Validate(bool aCheapValidateFlag)
		push 1
		mov eax, [ecx]
		call [eax + 0x10]
		test al, al

		pop ecx
		jz InvalidTarget

		// Target was actually valid if we reach here
		jmp [dwIsValidTargetJump]

	InvalidTarget:
		jmp [dwIsInvalidTargetJump]
	}
}