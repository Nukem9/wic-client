#include "WICP_MegaMap.h"

void WICP_MegaMap::EndDZPlacing(bool aFlyToDZFlag, bool aIsCancelledFlag)
{
	((void(__stdcall *)(WICP_MegaMap *, bool, bool))0x8F7880)(this, aFlyToDZFlag, aIsCancelledFlag);
}