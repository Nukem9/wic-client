#include "stdafx.h"

void PatchMemoryAllocators();

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

void __declspec(naked) hk_EX_OptionsModHandler__HandleEvent__SetActiveMod()
{
	__asm
	{
		// eax contains selected mod id
		// ecx contains EX_App::ourInstance
		mov ecx, dword ptr ds:[0x00E1E664]
		mov dword ptr ds:[ecx + 0x10F0], eax

		// EX_AppStateNavigator::SetEvent(EVENT_SET_ACTIVE_MOD)
		mov eax, 0xD
		lea edx, dword ptr ds:[ecx + 0x10EC]
		mov ecx, 0x00B42D00
		call ecx

		mov al, 0x1
		retn 0x8
	}
}

const char *__fastcall hk_MF_File__ExtractExtension(void *Unused, const char *aPath)
{
	if (!aPath)
		return "";

	auto ext = PathFindExtensionA(aPath);

	if (ext[0] == '.')
		return &ext[1];

	return ext;
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
	// Fix an out of bounds access when files without an extension are present in the game or mod folders
	//
	Detours::X86::DetourFunction((uint8_t *)0x009FCBB0, (uint8_t *)&hk_MF_File__ExtractExtension);

	//
	// Fix a use-after-free where they incorrectly hold a pointer to a MC_Str<> after it has exited scope (aka destructed). This is a tiny memory leak now.
	//
	PatchMemory(0x004287BD, (uint8_t *)"\x90\x90\x90\x90\x90", 5);

	//
	// Fix a bug where mods were loaded syncrhonously in a GUI handler when they should've been using the app event queue. MG_Gui::UpdateInternal
	// or MG_Gui::Update would crash after 'MG_Gui *this' was deleted INSIDE its own event handler.
	//
	Detours::X86::DetourFunctionClass((uint8_t *)0x00B4FC10, &hk_EX_OptionsModHandler__HandleEvent__SetActiveMod, Detours::X86Option::USE_CALL);
	Detours::X86::DetourFunctionClass((uint8_t *)0x00B4FC69, &hk_EX_OptionsModHandler__HandleEvent__SetActiveMod, Detours::X86Option::USE_CALL);

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
	Detours::X86::DetourFunctionClass((uint8_t *)0x00944A2E, &WICP_DebugView::Init, Detours::X86Option::USE_CALL);

	//
	// Restrict MSB_IoCore to 8 threads instead of (num processor cores * 8) threads
	//
	PatchMemory(0x00B6E7D3, (uint8_t *)"\x08", 1);

	//
	// Hook gethostbyname (IAT)
	//
	uintptr_t addr = (uintptr_t)&hk_gethostbyname;
	PatchMemory(0x00BEC594, (PBYTE)&addr, sizeof(uintptr_t));

	MN_NetRequester::InitializeHook();
	MMG_CdKey::InitializeHook();
	PatchMemoryAllocators();

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	return Wic_HookInit(hModule, ul_reason_for_call);
}