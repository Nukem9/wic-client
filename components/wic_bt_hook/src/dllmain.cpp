#include "stdafx.h"

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

void WicBT_HookInit(HMODULE hModule)
{
	if (_stricmp(reinterpret_cast<const char *>(0x00D35C62), "Built by daniel.wesslen/ME633 at 10:43:54 on Jun 22 2009 with compiler version 1400.\n") != 0)
	{
		MessageBoxA(nullptr, "Unknown broadcast tool version detected. Version 1.0.1.1 is required.", "Error", MB_ICONERROR);
		return;
	}

	//
	// Game counterpart. Fix a bug where mods were loaded synchronously in a GUI handler when they should've been using the app event queue. MG_Gui::UpdateInternal
	// or MG_Gui::Update would crash after 'MG_Gui *this' was deleted INSIDE its own event handler.
	//
	EX_OptionsModHandler::InitializeHook();

	//
	// Game counterpart. Write MMG_AccountProtocol cipher keys directly after EncryptionKeySequenceNumber in message packets.
	//
	MMG_AccountProtocol::InitializeHook();

	//
	// Game counterpart. Fix an out of bounds access when files without an extension are present in the game or mod folders.
	//
	MF_File::InitializeHook();

	//
	// Game counterpart. Always enable the console.
	//
	PatchMemory(0x00B3C396, { 0xEB });

	//
	// Game counterpart. Fix a use-after-free where they incorrectly hold a pointer to a MC_Str<> after it has exited scope (aka destructed). This is a tiny memory leak now.
	//
	PatchMemory(0x004289CD, { 0x90, 0x90, 0x90, 0x90, 0x90 });

	//
	// Game counterpart. Restrict MSB_IoCore to 8 threads instead of (num processor cores * 8) threads.
	//
	PatchMemory(0x00B79A03, { 0x08 });

	//
	// Game counterpart. Force the default server list filters to "no filter".
	//
	PatchMemory(0x008625D4, { 0x0 });
	PatchMemory(0x008625E1, { 0x0 });
	PatchMemory(0x008625EE, { 0x0 });

	//
	// Hook gethostbyname (IAT)
	//
	auto addr = reinterpret_cast<uintptr_t>(&hk_gethostbyname);
	PatchMemory(0x00BFB594, reinterpret_cast<uint8_t *>(&addr), sizeof(addr));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	WicBT_HookInit(hModule);
	return TRUE;
}