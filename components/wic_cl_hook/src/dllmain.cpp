#include "stdafx.h"

void WriteEncryptionKeys(void *accountQuery, MN_WriteMessage *aMessage)
{
	auto myCipher = *(MMG_BlockTEA **)((uintptr_t)accountQuery + 0x4EA);

	for(int i = 0; i < ARRAYSIZE(myCipher->myEncryptionKey); i++)
		aMessage->WriteUInt(myCipher->myEncryptionKey[i]);
}

void __declspec(naked) hk_MMG_AccountProtocol__Query__ToStream()
{
	__asm
	{
		push esi
		push ebx
		call WriteEncryptionKeys
		add esp, 0x8

		mov cl, byte ptr ds:[0x0DFDA6A]
		xor eax, eax

		push 0x00BD1C7B
		retn
	}
}

struct hostent *PASCAL hk_gethostbyname(const char *name)
{
	if (strstr(name, "massgate.net") ||
		strstr(name, "massive.se") ||
		strstr(name, "ubisoft.com"))
	{
		// Everything is redirected to a single IP address now
		return gethostbyname("liveaccount.massgate.org");
	}

	return gethostbyname(name);
}

BOOL Wic_HookInit(HMODULE hModule, DWORD ul_reason_for_call)
{
	if (_stricmp((const char *)0x00D2738C, "henrik.davidsson/MSV-BUILD-04 at 10:51:42 on Jun 10 2009.\n") != 0)
	{
		MessageBoxA(nullptr, "Unknown game version detected. Version 1.0.1.1 is required.", "Error", MB_ICONERROR);
		return FALSE;
	}

	//MMG_Protocols::MassgateProtocolVersion = 150;

	//
	// Always enable the console
	//
	PatchMemory(0x00B31A16, (uint8_t *)"\xEB", 1);

	//
	// Copy MC_Debug::DebugMessage strings directly to the console output
	//
	Detours::X86::DetourFunction((uint8_t *)0x00A01520, (uint8_t *)&EX3D_Console::StuffTextStatic);

	//
	// Write MMG_AccountProtocol cipher keys directly after EncryptionKeySequenceNumber in message packets
	//
	Detours::X86::DetourFunction((uint8_t *)0x00BD1C73, (uint8_t *)&hk_MMG_AccountProtocol__Query__ToStream);

	//
	// Register custom commands to show the ingame debug menu
	//
	Detours::X86::DetourFunctionClass((uint8_t *)0x008FF040, &WICP_DebugView::hk_Init);

	//
	// Hook gethostbyname (IAT)
	//
	uintptr_t addr = (uintptr_t)&hk_gethostbyname;
	PatchMemory(0x00BEC594, (PBYTE)&addr, sizeof(uintptr_t));

	MN_NetRequester::InitializeHook();
	MMG_CdKey::InitializeHook();

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	return Wic_HookInit(hModule, ul_reason_for_call);
}