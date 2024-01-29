#include <Hooks/Hooks.h>
#include "MI_Time.h"

//
// Patch out Massive's flawed code for detecting QueryPerformanceCounter overflows. It's completely
// inaccurate at high frame rates and they simply don't happen in modern OSes. Also, 2^64 nanoseconds
// is 500 years.
//
DECLARE_HOOK_INITIALIZER_MP(MI_Time,
{
	Hooks::WriteJump(0x009FF990, &MI_Time::GetQPC);
});

DECLARE_HOOK_INITIALIZER_DS(MI_Time,
{
	Hooks::WriteJump(0x0041ACB0, &MI_Time::GetQPC);
});