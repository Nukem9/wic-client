#include <Hooks/Hooks.h>
#include "MM_AngleUtils.h"

//
// Reimplement the following mathematical functions. One reason is for simplified debugging. The
// other is to correctly handle radians outside the range of [-2pi, 2pi].
//
// These aren't hooked in the client because they're all inlined.
//
DECLARE_HOOK_INITIALIZER_DS(MM_AngleUtils,
{
	Hooks::WriteJump(0x0042B830, &MM_AngleUtils::CropAngle);
	Hooks::WriteJump(0x0042B860, &MM_AngleUtils::CropAngleDiff);
	Hooks::WriteJump(0x0042B8A0, &MM_AngleUtils::AngleDiff);
	Hooks::WriteJump(0x0042B910, &MM_AngleUtils::MoveToAngle);
});