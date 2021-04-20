#pragma once

class MI_Time 
{
public:
	CLASSPTR(MI_Time *, 0x008BF83C, ourInstance);
	CLASSPTR(uint32_t,	0x008BF820, ourFrameCounter);
	CLASSPTR(float,		0x008BF824, ourCurrentTime);
	CLASSPTR(double,	0x008BF828, ourRealCurrentTime);
	CLASSPTR(double,	0x008BF830, ourRealElapsedTime);
	CLASSPTR(float,		0x008BF838, ourElapsedTime);
	CLASSPTR(uint64_t,	0x008BF840, ourStartTimes);

	double	*ourInternalElapsedTimes;	// this+0x0
	double	ourInternalCurrentTime;		// this+0x8
	int		myCurrentTimeIndex;			// this+0x10
	float	myMinElapsedTime;			// this+0x14
	float	myMaxElapsedTime;			// this+0x18
	bool	myPauseFlag;				// this+0x1C
	float	myTimeScale;				// this+0x20
};

CHECK_OFFSET(MI_Time, ourInternalElapsedTimes, 0x0);
CHECK_OFFSET(MI_Time, ourInternalCurrentTime, 0x8);
CHECK_OFFSET(MI_Time, myCurrentTimeIndex, 0x10);
CHECK_OFFSET(MI_Time, myMinElapsedTime, 0x14);
CHECK_OFFSET(MI_Time, myMaxElapsedTime, 0x18);
CHECK_OFFSET(MI_Time, myPauseFlag, 0x1C);
CHECK_OFFSET(MI_Time, myTimeScale, 0x20);