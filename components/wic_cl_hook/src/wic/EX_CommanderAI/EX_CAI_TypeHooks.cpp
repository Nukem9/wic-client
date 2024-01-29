#include <Hooks/Hooks.h>
#include "EX_CAI_Type.h"

//
// Bypass assertions while playing MWMod with bots enabled on the dedicated server
//
DECLARE_HOOK_INITIALIZER_DS(EX_CAI_Type,
{
	Hooks::WriteJump(0x006CA630, &EX_CAI_Type::GetWeaponDamageDirect);
	Hooks::WriteJump(0x006CA6C0, &EX_CAI_Type::GetWeaponDamageBlast);
	Hooks::WriteJump(0x006CA750, &EX_CAI_Type::GetArmorPiercingDirect);
	Hooks::WriteJump(0x006CA7E0, &EX_CAI_Type::GetArmorPiercingBlast);
	Hooks::WriteJump(0x006CA870, &EX_CAI_Type::GetFiringRate);
	Hooks::WriteJump(0x006CA900, &EX_CAI_Type::GetBulletsPerMag);
	Hooks::WriteJump(0x006CA990, &EX_CAI_Type::GetReloadTime);
	Hooks::WriteJump(0x006CAA20, &EX_CAI_Type::GetAccuracy);
});