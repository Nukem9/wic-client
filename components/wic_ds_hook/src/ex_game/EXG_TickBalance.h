#pragma once

class EXG_TickBalance
{
public:
	static void InitializeHook();

private:
	static double Update_00501B99(double Value);
};