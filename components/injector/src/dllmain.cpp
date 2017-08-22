#include "stdafx.h"

wchar_t g_ModulePath[MAX_PATH];
HMODULE g_ModuleHandle;

uint8_t *detourInfo;
uint8_t *detourEntry;

int __cdecl hk_mainCRTStartup()
{
	// Check which game exe this is (multiple variations, same version)
	if (wcsstr(g_ModulePath, L"WICEd"))
	{
		// WIC map editor
	}
	if (wcsstr(g_ModulePath, L"wic_ds"))
	{
		// Dedicated server
		LoadLibrary(L"wic_ds_hook.dll");
	}
	else if (wcsstr(g_ModulePath, L"wic-BroadcastTool"))
	{
		// Broadcast (recording) tool
		LoadLibrary(L"wic_bt_hook.dll");
	}
	else if (wcsstr(g_ModulePath, L"wic_online"))
	{
		// Vanilla game exe, multiplayer-mode only
		//
		// This exe is pointless, so relaunch wic.exe that has actual patches
		if (!RelaunchWicExecutable(L"wic", L"wic_online"))
			MessageBox(nullptr, L"Unable to launch 'wic.exe'", L"Injector error", MB_ICONERROR);

		ExitProcess(0);
	}
	if (wcsstr(g_ModulePath, L"wic"))
	{
		// Vanilla game exe
		LoadLibrary(L"wic_cl_hook.dll");
	}
	else
	{
		MessageBox(nullptr, L"Unknown game executable detected. DLL not loaded.", L"Injector error", MB_ICONERROR);
	}

	// Unhook and return to the original function prologue
	Detours::X86::DetourRemove(detourInfo);
	return ((int(__cdecl *)())detourEntry)();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	DisableThreadLibraryCalls(hModule);

	// Determine the executable path
	g_ModuleHandle = GetModuleHandle(nullptr);

	if (!GetModuleFileName(g_ModuleHandle, g_ModulePath, ARRAYSIZE(g_ModulePath)))
		return FALSE;

	// Determine if the EXE version is 1.0.1.1
	int major;
	int minor;
	int build;
	int revision;

	if (!GetModuleVersion(g_ModulePath, &major, &minor, &build, &revision))
		return FALSE;

	if (major != 1 || minor != 0 || build != 1 || revision != 1)
		return FALSE;

	// Now hook the original entrypoint
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)g_ModuleHandle;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t *)dosHeader + dosHeader->e_lfanew);
	uint8_t *entryAddress = ((uint8_t *)dosHeader + ntHeaders->OptionalHeader.AddressOfEntryPoint);

	detourInfo = Detours::X86::DetourFunction(entryAddress, (uint8_t *)&hk_mainCRTStartup);
	detourEntry = entryAddress;
	return TRUE;
}