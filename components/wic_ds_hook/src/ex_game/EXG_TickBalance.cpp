#include "../stdafx.h"

void EXG_TickBalance::InitializeHook()
{
	Detours::X86::DetourFunctionClass(0x00501B99, &Update_00501B99, Detours::X86Option::USE_CALL);
}

double EXG_TickBalance::Update_00501B99(double Value)
{
	if (Value < 0)
		return floor(Value);

	return ceil(Value);
}