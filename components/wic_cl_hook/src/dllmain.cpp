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

void *operator new(size_t size)
{
	return ((void *(__cdecl *)(size_t))0x00B2DDE0)(size);
}

void operator delete(void *ptr)
{
	((void(__cdecl *)(void *))0x00B2DDA0)(ptr);
}

bool __fastcall Wic_ParseCommandLine(const char *CommandLine)
{
	// The listener must be registered after CRT initialization, but during early init, so do it here
	auto listener = new MC_DebugConsoleListener();
	((bool(__thiscall *)(void *))0x00A01780)(listener);

	if (((bool(__thiscall *)(const char *))0x00B2F6A0)(CommandLine))
	{
		if (MC_CommandLine::ourInstance->IsPresent("ignorealttab"))
			PatchMemory(0x00B2E101, { 0xE9, 0x1E, 0x01, 0x00, 0x00 });

		if (MC_CommandLine::ourInstance->IsPresent("nocursorspeed"))
			PatchMemory(0x0098C1B1, { 0xEB });

		return true;
	}

	return false;
}

void Wic_HookInit(HMODULE hModule)
{
	if (_stricmp(reinterpret_cast<const char *>(0x00D2738C), "henrik.davidsson/MSV-BUILD-04 at 10:51:42 on Jun 10 2009.\n") != 0)
	{
		MessageBoxA(nullptr, "Unknown game version detected. Version 1.0.1.1 is required.", "Error", MB_ICONERROR);
		return;
	}

	//
	// Fix a crash from a null pointer dereference when EX_CAI_CommanderAI_Data::CompareEnemyGroups has no groups available
	//
	EX_CAI_Platoon::InitializeHook();

	//
	// Assert and kill the game if a unit has more than 8 active shooters. The server and client support units with more than 8 shooters, but the
	// networking code doesn't. This causes memory corruption otherwise.
	//
	EXR_Server::InitializeHook();

	//
	// Fix an out of bounds access when files without an extension are present in the game or mod folders
	//
	MF_File::InitializeHook();

	//
	// Fix a bug where mods were loaded synchronously in a GUI handler when they should've been using the app event queue. MG_Gui::UpdateInternal
	// or MG_Gui::Update would crash after 'MG_Gui *this' was deleted INSIDE its own event handler.
	//
	EX_OptionsModHandler::InitializeHook();

	//
	// Write MMG_AccountProtocol cipher keys directly after EncryptionKeySequenceNumber in message packets
	//
	MMG_AccountProtocol::InitializeHook();

	//
	// Register custom commands to show the ingame debug menu
	//
	WICP_DebugView::InitializeHook();

	//
	// Rewrite old URL hostnames so HTTP request headers are correct
	//
	MN_NetRequester::InitializeHook();

	//
	// Rewrite CDKEY validation logic
	//
	MMG_CdKey::InitializeHook();

	//
	// Fix for the rounding error in team tick rates when playing domination ("Dom Bar Bug"). Under certain conditions, such as when NonLinearDomination
	// is enabled, one team would have a small advantage throughout the match. This varies depending on the map parameters and captured command points.
	//
	// Simplified equation:
	// C1                = # of captured command points by Team 1
	// C2                = # of captured command points by Team 2
	// NonLinearDomScale = Arbitrary point on a spline (between 0 and 1)
	// TickRate          = abs( ceil( (C1 - C2) * NonLinearDomScale ) )
	//
	// Example 1 (Team 1 gaining ticks):
	// C1                = 4
	// C2                = 1
	// NonLinearDomScale = 0.95
	// TickRate          = abs(ceil((4 - 1) * 0.95)) = abs(ceil(2.85)) = 3 ticks per second 
	//
	// Example 2 (Team 2 gaining ticks) (Command point counts swapped):
	// C1                = 1
	// C2                = 4
	// NonLinearDomScale = 0.95
	// TickRate          = abs(ceil((1 - 4) * 0.95)) = abs(ceil(-2.85)) = 2 ticks per second
	//
	// Team 2 is at a disadvantage because ceil() rounds **negative** numbers towards zero. The patch applies floor() to negative numbers (Example 2) and
	// ceil() to positive numbers (Example 1) so both teams would have an equal tick rate. This bug has been present since the game was released in 2007.
	//
	// Special thanks to Cat, Gut Plaer, and the community for investigation. Confirmed as a bug by former WIC developers.
	//
	EXG_TickBalance::InitializeHook();

	//
	// Hook an early function in WinMain to do a few things:
	// - Register a MC_Debug listener to copy strings directly to the console output
	// - Add "-ignorealttab" as a command line option to prevent the game from going idle
	// - Add "-nocursorspeed" as a command line option to prevent the game from setting mouse sensitivity
	//
	Detours::X86::DetourFunctionClass(reinterpret_cast<uint8_t *>(0x00B2EF85), &Wic_ParseCommandLine, Detours::X86Option::USE_CALL);

	//
	// Always enable the console
	//
	PatchMemory(0x00B31A16, { 0xEB });

	//
	// Fix a use-after-free where they incorrectly hold a pointer to a MC_Str<> after it has exited scope (aka destructed). This is a tiny memory leak now.
	//
	PatchMemory(0x004287BD, { 0x90, 0x90, 0x90, 0x90, 0x90 });

	//
	// Restrict MSB_IoCore to 8 threads instead of (num processor cores * 8) threads
	//
	PatchMemory(0x00B6E7D3, { 0x08 });

	//
	// Force the default server list filters to "no filter"
	//
	PatchMemory(0x00863114, { 0x0 });
	PatchMemory(0x00863121, { 0x0 });
	PatchMemory(0x0086312E, { 0x0 });

	//
	// Hook gethostbyname (IAT)
	//
	auto addr = reinterpret_cast<uintptr_t>(&hk_gethostbyname);
	PatchMemory(0x00BEC594, reinterpret_cast<uint8_t *>(&addr), sizeof(addr));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	Wic_HookInit(hModule);
	return TRUE;
}