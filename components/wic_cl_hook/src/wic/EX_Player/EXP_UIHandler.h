#pragma once

#include "../EX_Common/EXCO_Constants.h"

class EXP_UIHandler
{
public:
	char _padding[0x22];
	bool myHasSetDZFlag;

	void PostScreenMessage(const wchar_t *aMessage, EXCO_Constants::ScreenMessageType aMsgType, float aTime);
};
