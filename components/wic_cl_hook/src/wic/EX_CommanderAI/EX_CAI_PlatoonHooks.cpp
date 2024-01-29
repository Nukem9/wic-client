#include <Hooks/Hooks.h>
#include <MCommon2/MC_Debug.h>

void hk_call_00807D34();
void hk_006C7DB9();

//
// 1. Fix a crash from a null pointer dereference when EX_CAI_CommanderAI_Data::CompareEnemyGroups has
// no groups available. The debug log acknowledges the error, but they dereference it anyway. Function:
// EX_CAI_Platoon::AssesSituation.
//
// 2. Fix a memory leak in EX_CAI_Platoon::Update. When a bot server is idle with no real players, this
// function will continuously add EX_CAI_RESOURCE_REQUEST orders to an array. Memory usage keeps growing
// until it crashes.
//
DECLARE_HOOK_INITIALIZER_MP(EX_CAI_Platoon,
{
	Hooks::WriteJump(0x00807D34, &hk_call_00807D34);
	Hooks::WriteJump(0x0080C423, &hk_006C7DB9);
});

DECLARE_HOOK_INITIALIZER_DS(EX_CAI_Platoon,
{
	Hooks::WriteJump(0x006C70BF, &hk_call_00807D34);
	Hooks::WriteJump(0x006C7DB9, &hk_006C7DB9);
});

void __declspec(naked) hk_call_00807D34()
{
	const static auto dwSkipOffset = SelectOffset(0x00807D6B, 0x006C714B);
	const static auto internalPosTracerCallback = &MC_Debug::InternalPosTracer::operator();
	static_assert_size(internalPosTracerCallback, 0x4);

	__asm
	{
		call [internalPosTracerCallback]
		add esp, 0x10

		jmp [dwSkipOffset]
	}
}

void __declspec(naked) hk_006C7DB9()
{
	const static u32 dwDoResourceRequest = SelectOffset(0x0080C429, 0x006C7DBE);
	const static u32 dwSkipResourceRequest = SelectOffset(0x0080C4BB, 0x006C7DEE);

	__asm
	{
		jz SkipResourceRequest

		mov edx, [ebp + 0xA74] // (EX_CAI_Platoon *this)->myGameWrapper
		mov dl, [edx + 0x40] // (EX_CAI_GameWrapper *this)->myCanSpawnFlag
		test dl, dl
		jz SkipResourceRequest

		mov edx, [ebp + 0x4]
		jmp [dwDoResourceRequest]

	SkipResourceRequest:
		jmp [dwSkipResourceRequest]
	}
}