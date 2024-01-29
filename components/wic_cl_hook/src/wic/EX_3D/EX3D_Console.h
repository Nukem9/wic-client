#pragma once

#include <Hooks/Memory.h>

class MR_Console;

class EX3D_Console
{
public:
	static inline DeclareGlobalRef(EX3D_Console *, ourInstance, SelectOffset(0x00E147A4, 0x0));

	MR_Console *myConsole;

	void StuffText(const char *aString, ...);
	void AddFunc(const char *aName, bool(__cdecl *aFuncPtr)(char *, void *), void *aUserData, bool aUseFileTabCompletion, char *aFileExtension);
};