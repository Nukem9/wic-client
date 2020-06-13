#include "../stdafx.h"

DPTR(EX3D_Console, ourInstance);

void EX3D_Console::StuffText(const char *aString, ...)
{
	char buffer[2048];
	va_list va;

	va_start(va, aString);
	_vsnprintf_s(buffer, _TRUNCATE, aString, va);
	va_end(va);

	((void(__cdecl *)(EX3D_Console *, const char *, ...))0x00AFDB90)(this, buffer);
}

void EX3D_Console::AddFunc(const char *aName, bool(__cdecl *aFuncPtr)(char *, void *), void *aUserData, bool aUseFileTabCompletion, char *aFileExtension)
{
	static uintptr_t dwCall = 0x00A789F0;

	void *mrConsole = myConsole;

	__asm
	{
		movzx eax, aUseFileTabCompletion
		push eax
		push aUserData
		push aFuncPtr
		mov eax, aName
		mov ecx, mrConsole
		call [dwCall]
	}
}