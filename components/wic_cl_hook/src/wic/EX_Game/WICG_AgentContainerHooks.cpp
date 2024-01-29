#include <Hooks/Hooks.h>
#include <EX_Common/EXCO_Constants.h>

void hk_004C7C28();

//
// WICG_AgentContainer::GetUnitsInRangeXZ has a bug where the developers incorrectly pass a non-unit
// agent to EXCO_LOSSet::IsVisible. EXCO_LOSSet expects units only, which means IDs in the interval of
// [0, 512). Agents such as buildings have IDs in the interval [512, 65536).
// 
// These aagent IDs can cause out of bounds array access(es). In the best case, it's benign and nothing
// happens. In the worst case, the server crashes.
//
DECLARE_HOOK_INITIALIZER_MP(WICG_AgentContainer,
{
	Hooks::WriteJump(0x0070FAF0, &hk_004C7C28);
});

DECLARE_HOOK_INITIALIZER_DS(WICG_AgentContainer,
{
	Hooks::WriteJump(0x004C7C28, &hk_004C7C28);
});

void __declspec(naked) hk_004C7C28()
{
	const static auto dwValidUnitJump = SelectOffset(0x0070FAFB, 0x004C7C2F);
	const static auto dwInvalidUnitJump = SelectOffset(0x0070FB1A, 0x004C7C3B);

	__asm
	{
#if WIC_MP
		movzx eax, word ptr ds:[eax + 0xC0] // WICG_Agent::myId
		lea edx, [ebp + edx * 4]
#elif WIC_DS
		movzx eax, word ptr ds:[edx + 0xC0] // WICG_Agent::myId
#endif
		cmp eax, EX_MAX_NUM_UNITS
		jge AgentIsNotAUnit
		jmp [dwValidUnitJump]

	AgentIsNotAUnit:
		jmp [dwInvalidUnitJump]
	}
}