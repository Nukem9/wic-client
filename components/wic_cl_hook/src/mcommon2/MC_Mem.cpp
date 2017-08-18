#include "../stdafx.h"

void MC_Assert(const char *aFile, int aLine, const char *aString, bool *aIgnoreFlag)
{
	if(aIgnoreFlag && *aIgnoreFlag)
		return;

	char buffer[2048];
	sprintf_s(buffer, "ASSERTION FAILED:\n\n%s(%d): %s\n", aFile, aLine, aString);

	MessageBox(nullptr, buffer, "Assertion", MB_ICONERROR);
	ExitProcess(0);
}