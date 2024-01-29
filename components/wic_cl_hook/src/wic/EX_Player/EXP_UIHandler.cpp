#include "EXP_UIHandler.h"

void EXP_UIHandler::PostScreenMessage(const wchar_t *aMessage, EXCO_Constants::ScreenMessageType aMsgType, float aTime)
{
	((void(__thiscall *)(const wchar_t *, EXCO_Constants::ScreenMessageType, float))0x00949350)(aMessage, aMsgType, aTime);
}