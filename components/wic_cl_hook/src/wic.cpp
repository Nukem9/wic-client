#include <Hooks/Hooks.h>
#include <Hooks/Memory.h>
#include <Windows.h>
#include <MCommon2/MC_CommandLine.h>
#include <EX_3D/EX3D_DebugConsoleListener.h>
#include <MCommon2/MF_File.h>
#include <MCommon2/MC_Debug.h>

bool __fastcall Wic_ParseCommandLine(const char *CommandLine)
{
	// The listener must be registered after CRT initialization, but during early init, so do it here
	auto listener = EX3D_DebugConsoleListener::Create();
	MC_Debug::AddDebugListener(listener);

	if (((bool(__thiscall *)(const char *))0x00B2F6A0)(CommandLine))
	{
		if (MC_CommandLine::GetInstance()->IsPresent("ignorealttab"))
			Memory::Patch(0x00B2E101, { 0xE9, 0x1E, 0x01, 0x00, 0x00 });

		if (MC_CommandLine::GetInstance()->IsPresent("nocursorspeed"))
			Memory::Patch(0x0098C1B1, { 0xEB });

		return true;
	}

	return false;
}

void WriteCustomSetting(MF_File *File)
{
	char buffer[1024];
	
	auto bgra = (uint8_t *)0x00EA2860;
	sprintf_s(buffer, "myCustomUnitColor %d:%d:%d\r\n", (int)bgra[0], (int)bgra[1], (int)bgra[2]);

	File->Write(buffer, strlen(buffer));
}

void LoadCustomSetting(const char *Name, const char *Value)
{
	if (_stricmp(Name, "myCustomUnitColor") == 0)
	{
		int r, g, b;
		sscanf_s(Value, "%d:%d:%d", &b, &g, &r);

		auto bgra = (uint8_t *)0x00EA2860;
		bgra[0] = b;
		bgra[1] = g;
		bgra[2] = r;
	}
}

void __declspec(naked) hk_0099942A()
{
	const static uint32_t dwCall = 0x428790;

	__asm
	{
		// Write the custom setting
		pushad
		push ecx
		call WriteCustomSetting
		add esp, 0x4
		popad

		// Write the original configuration setting
		call dword ptr [dwCall]

		// Jump back to the original code
		push 0x0099942F
		retn
	}
}

void __declspec(naked) hk_0099AEAE()
{
	__asm
	{
		pushad
		push ebx
		push ebp
		call LoadCustomSetting
		add esp, 0x8
		popad

		mov edi, 0x00D34188
		push 0x0099AEB3
		retn
	}
}

#include <mimalloc.h>

void dofree(void *ptr)
{
	free(ptr);
	//mi_free(ptr);
}

void __declspec(naked) hk_MC_FastFree()
{
	__asm
	{
		push esi
		call dofree
		add esp, 0x4
		retn
	}
}

DECLARE_HOOK_INITIALIZER_MP(TestingZone,
{
	//return;

	Hooks::WriteJump(0x005567E0, &memmove);
	Hooks::WriteJump(0x0055B5C0, &memcpy);
	Hooks::WriteJump(0x0055B960, &memset);


	Hooks::WriteJump(0x00B2DDE0, &malloc);// operator new
	Hooks::WriteJump(0x00B2DDA0, &dofree);// operator delete
	Hooks::WriteJump(0x00B2DD60, &dofree);// operator delete[]
	Hooks::WriteJump(0x009F3BC0, &hk_MC_FastFree);// MC_FastFree
	Memory::Patch(0x00B2F01F, { 0xEB });// force locTempMemEnableFlag off

	//
	// Hook an early function in WinMain to do a few things:
	// - Register a MC_Debug listener to copy strings directly to the console output
	// - Add "-ignorealttab" as a command line option to prevent the game from going idle
	// - Add "-nocursorspeed" as a command line option to prevent the game from setting mouse sensitivity
	//
	Hooks::WriteCall(0x00B2EF85, &Wic_ParseCommandLine);

	// Always enable the console
	Memory::Patch(0x00B31A16, { 0xEB });

	// Force the default server list filters to "no filter"
	Memory::Patch(0x00863114, { 0x0 });
	Memory::Patch(0x00863121, { 0x0 });
	Memory::Patch(0x0086312E, { 0x0 });



	// Restrict MSB_IoCore to 4 threads instead of (num processor cores * 8) threads
	Memory::Patch(0x00B6E7D3, { 0x4 });

	// Patch for WICG_MPyPlayer::cPlayer_ChatMessage where the developers incorrectly used player slot #0 instead
	// of the script player when sending chat messages. Slot 0 will crash if no player is connected. Slot 255 is
	// used for scripts.
	//
	// NOTE: The MP LAN lobby assigns the host player to slot 0. It'll always be connected. We're keeping it for
	// parity with the dedicated server.
	Memory::Patch(0x00724CC2, { 0x90, 0x90 });
		

	// Option to disable TA taunts
	// Option to set default formation
	// Option to set team color
	// Option to set enemy color
	// Option to set player color
	// Option to set new chat style
	// Option to set number of chat lines
	// Option to show nuke ta warning


	// Disable access to the windows registry from python scripts
	Memory::Fill(0x00DDDBD0, 0x0, 288);

	// Disable shader cache source file comparisons as they're 100% useless. The file name is already a hash of
	// its contents. Massive decided to use a string comparison on top of that.
	Memory::Patch(0x00A8017C, { 0xE9, 0xCA, 0x00, 0x00, 0x00 });

	// Disable MAdHoc. It's a useless feature and only serves to cause problems. Windows 10 removed support
	// entirely.
	Memory::Patch(0x009D999A, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x31, 0xC0 });

	// Disable windows error reporting (FaultRep) submissions. There's nobody listening on the other end.
	Memory::Patch(0x009FEB10, { 0xC3 });

	// Disable spammy debug messages when delay-load sounds are used: "MS2_Utility::GetSound() Delayed load for
	// sound %d".
	Memory::Patch(0x00BA752C, { 0x90, 0x90, 0x90, 0x90, 0x90 });
});

static bool TestVersion()
{
	__try
	{
		auto testValue = reinterpret_cast<const char *>(0x00D2738C);
		auto expectedValue = "henrik.davidsson/MSV-BUILD-04 at 10:51:42 on Jun 10 2009.\n";

		return memcmp(testValue, expectedValue, strlen(expectedValue)) == 0;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	return false;
}

void WIC_HookInit(HMODULE Module)
{
	if (!TestVersion())
	{
		MessageBoxA(nullptr, "Unknown game version detected. Version 1.0.1.1 is required.", "Error", MB_ICONERROR);
		ExitProcess(1);
	}

	if (!Hooks::Initialize())
	{
		MessageBoxA(nullptr, "Fatal error during hook initialization", "Error", MB_ICONERROR);
		ExitProcess(1);
	}

	//
	// Fix a use-after-free where they incorrectly hold a pointer to a MC_Str<> after it has exited scope (aka destructed). This is a tiny memory leak now.
	//
	Memory::Patch(0x004287BD, { 0x90, 0x90, 0x90, 0x90, 0x90 });

	/*
	Detours::X86::DetourFunctionT(0x0099AEAE, &hk_0099AEAE, Detours::X86Option::USE_JUMP);// read
	Detours::X86::DetourFunctionT(0x0099942A, &hk_0099942A, Detours::X86Option::USE_JUMP);// write
	Patch(0x00EA2860, { 0xFF, 161, 167, 79 });// default mySelfPlayerColors_myHighlightColor to green if not set in the config
	Patch(0x009263EC, { 0x90, 0x90, 0x90, 0x90, 0x90 });// prevent WICP_VisualConfig::Reload from assigning its default value
	*/
}