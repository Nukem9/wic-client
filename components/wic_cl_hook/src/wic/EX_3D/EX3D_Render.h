#pragma once

#include <MCommon2/MC_GrowingArray.h>
#include <MCommon2/MC_String.h>

class MR_EnumerationOutput;

class MR_EnumerationDevice
{
public:
	MC_GrowingArray<MR_EnumerationOutput *> myOutputs;
	u32 myMemorySize;
	bool myStencilBufferFlag;
	bool myAlphaBufferFlag;
	s32 myDeviceId;
	MC_String myName;
};
static_assert_size(MR_EnumerationDevice, 0x20);