#include "../stdafx.h"

char EXCO_Directory::ourModDirectory[MAX_PATH];

void EXCO_Directory::InitializeHook()
{
	// Hook so the directory doesn't point to 'My Documents'
	Detours::X86::DetourFunctionClass((PBYTE)0x0044C120, &GetModDirectory);

	// Current server directory
	DWORD bufferLen = GetCurrentDirectoryA(ARRAYSIZE(ourModDirectory), ourModDirectory);

	MC_ASSERT(bufferLen > 0);

	// Append the 'mods' folder path
	strcat_s(ourModDirectory, "\\mods\\");
}

const char *EXCO_Directory::GetModDirectory()
{
	return ourModDirectory;
}