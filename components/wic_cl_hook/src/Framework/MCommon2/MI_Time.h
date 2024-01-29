#pragma once

#include <Hooks/Memory.h>

using MI_TimeUnit = u64;

enum MI_TimerMethod
{
	MI_RDTSC,
	MI_TIMEGETTIME,
	MI_QPC,
};

class MI_Time
{
private:
	static inline DeclareGlobalRef(float, ourCurrentTime, SelectOffset(0x00E17CE4, 0x008BF824));

public:
	static float GetCurrentTime();
	static u32 GetSystemTime();
	static bool GetQPC(MI_TimeUnit *aReturnTime);
};