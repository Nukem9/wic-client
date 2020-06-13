#pragma once

class EX3D_Console
{
public:
	CLASSPTR(EX3D_Console *, 0x00E147A4, ourInstance);

	class MR_Console *myConsole;

	void StuffText(const char *aString, ...);
	void AddFunc(const char *aName, bool(__cdecl *aFuncPtr)(char *, void *), void *aUserData, bool aUseFileTabCompletion, char *aFileExtension);
};