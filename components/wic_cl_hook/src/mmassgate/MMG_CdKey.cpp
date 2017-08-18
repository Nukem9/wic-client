#include "../stdafx.h"

void MMG_CdKey::InitializeHook()
{
	Detours::X86::DetourFunctionClass((PBYTE)0x0079C3A0, &MMG_CdKey::Validator::hk_SetKey);
}

void MMG_CdKey::Validator::SetKey(const char *aKey)
{
	// Key is always valid
	this->myIsChecksumCorrect = true;

	if (aKey)
	{
		// "BJORN" is a special developer key and not supported
		if (strstr(aKey, "BJORN"))
			this->myIsChecksumCorrect = false;

		// Log this to the console
		printf("%s: %s\n", __FUNCTION__, aKey);
	}

	// Fill out a fake key for encryption purposes
	strcpy_s(this->myKey.data, "CUSTOMKEYCLIENT");
}

bool MMG_CdKey::Validator::IsKeyValid()
{
	return this->myIsChecksumCorrect;
}

void __declspec(naked) MMG_CdKey::Validator::hk_SetKey()
{
	__asm
	{
		push dword ptr [esp+4h]
		mov ecx, esi
		call offset MMG_CdKey::Validator::SetKey
		retn 4
	}
}