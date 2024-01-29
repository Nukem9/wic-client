#pragma once

class WICO_BotOptions
{
public:
	enum BotMode : s32
	{
		BOTMODE_NONE = 0x0,
		BOTMODE_EVEN = 0x1,
		BOTMODE_PVSB = 0x2,
		BOTMODE_ADVANCED = 0x3,
	};

	char _pad[0x134];
};
static_assert_size(WICO_BotOptions, 0x134);