#include "../stdafx.h"

void EXG_TickBalance::InitializeHook()
{
	Detours::X86::DetourFunctionClass(0x00705A12, &Update_00705A12, Detours::X86Option::USE_CALL);
}

double EXG_TickBalance::Update_00705A12(double Value)
{
	if (Value < 0)
		return floor(Value);

	return ceil(Value);
}