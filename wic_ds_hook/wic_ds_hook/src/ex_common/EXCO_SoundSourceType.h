#pragma once

#pragma pack(push, 1)
class EXCO_SoundSourceType
{
public:
	unsigned long	myBaseFrequency;		// this+0x0
	int				mySound;				// this+0x4
	bool			mySpeedShiftFlag;		// this+0x8
	bool			myDopplerShiftFlag;		// this+0x9
};
#pragma pack(pop)