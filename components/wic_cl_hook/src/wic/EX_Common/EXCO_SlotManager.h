#pragma once

#include <MCommon2/MC_GrowingArray.h>

class EXCO_Slot;

class EXCO_SlotManager
{
public:
	MC_GrowingArray<EXCO_Slot *> mySlots;
};
static_assert_size(EXCO_SlotManager, 0x10);