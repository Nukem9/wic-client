#pragma once

class EXCO_Difficulty
{
public:
	enum Level : s32
	{
		EASY = 0x0,
		NORMAL = 0x1,
		HARD = 0x2,
		VERYHARD = 0x3,
		NUM_DIFFICULTY_LEVELS = 0x4,
	};
};