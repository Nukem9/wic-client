#pragma once

class EXG_TickBalance
{
public:
	static void InitializeHook();

private:
	static double Update_00705A12(double Value);
};