#pragma once

class MC_CommandLine
{
public:
	CLASSPTR(MC_CommandLine *, 0x00E1F2C4, ourInstance);

	bool IsPresent(const char *Option);
};