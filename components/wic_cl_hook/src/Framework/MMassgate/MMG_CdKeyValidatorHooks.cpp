#include <Hooks/Hooks.h>
#include "MMG_CdKeyValidator.h"

void hk_MMG_CdKey__Validator__SetKey(const char *aKey);

//
// Patch out the CD key validation check. All key inputs are considered valid.
//
DECLARE_HOOK_INITIALIZER_MP(MMG_CdKeyValidator,
{
	Hooks::WriteJump(0x0079C3A0, &hk_MMG_CdKey__Validator__SetKey);
});

DECLARE_HOOK_INITIALIZER_DS(MMG_CdKeyValidator,
{
	Hooks::WriteJump(0x00485700, &MMG_CdKey::Validator::SetKey);
});

void __declspec(naked) hk_MMG_CdKey__Validator__SetKey(const char *aKey)
{
	MMG_CdKey::Validator *validator;

	__asm
	{
		push ebp
		mov ebp, esp
		sub esp, __LOCAL_SIZE
		mov validator, esi
	}

	validator->SetKey(aKey);

	__asm
	{
		leave
		retn 0x4
	}
}