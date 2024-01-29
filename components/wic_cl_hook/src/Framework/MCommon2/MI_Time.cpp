#include <Windows.h>
#undef GetCurrentTime

#include "MI_Time.h"

float MI_Time::GetCurrentTime()
{
	return ourCurrentTime;
}

u32 MI_Time::GetSystemTime()
{
	return timeGetTime();
}

bool MI_Time::GetQPC(MI_TimeUnit *aReturnTime)
{
	const static MI_TimeUnit firstQpc = []()
	{
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		return counter.QuadPart;
	}();

	LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);

	*aReturnTime = std::max(counter.QuadPart - firstQpc, 1ull);
	return true;
}