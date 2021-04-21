#pragma once

class EX_OptionsModHandler
{
public:
	static void InitializeHook();

private:
	static void HandleEvent__SetActiveMod_00B5A340();
};