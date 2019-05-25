#include "stdafx.h"

wchar_t g_ModulePath[1024];
wchar_t g_ExeName[1024];
HMODULE g_ModuleHandle;

uint8_t *detourInfo;
uint8_t *detourEntry;

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

void WINAPI hk_GetSystemInfo(LPSYSTEM_INFO lpSystemInfo)
{
	GetSystemInfo(lpSystemInfo);

	// Restrict core count to 16 because of a hardcoded array in MT_Supervisor
	if (lpSystemInfo)
		lpSystemInfo->dwNumberOfProcessors = min(lpSystemInfo->dwNumberOfProcessors, 16);
}

DWORD_PTR WINAPI hk_SetThreadAffinityMask(HANDLE hThread, DWORD_PTR dwThreadAffinityMask)
{
	// Don't change anything, the OS knows better than the game
	return 0xFFFFFFFF;
}

int __cdecl hk_mainCRTStartup()
{
	if (!_wcsicmp(g_ExeName, L"GuiEditor") ||
		!_wcsicmp(g_ExeName, L"JuiceMaker") ||
		!_wcsicmp(g_ExeName, L"ShowboxN") ||
		!_wcsicmp(g_ExeName, L"WICEd") ||
		!_wcsicmp(g_ExeName, L"WinSDF"))
	{
		// Modkit executables. Nothing specific to load.
	}
	else
	{
		if (!_wcsicmp(g_ExeName, L"wic_ds"))
		{
			// Dedicated server
			LoadLibrary(L"wic_ds_hook.dll");
		}
		else if (!_wcsicmp(g_ExeName, L"wic-BroadcastTool"))
		{
			// Broadcast (recording) tool
			LoadLibrary(L"wic_bt_hook.dll");
		}
		else if (!_wcsicmp(g_ExeName, L"wic_online"))
		{
			// Vanilla game exe, multiplayer-mode only
			//
			// This exe is pointless, so relaunch wic.exe that has actual patches
			if (!RelaunchWicExecutable(L"wic", L"wic_online"))
				MessageBox(nullptr, L"Unable to launch 'wic.exe'", L"Injector error", MB_ICONERROR);

			ExitProcess(0);
		}
		else if (!_wcsicmp(g_ExeName, L"wic"))
		{
			// Vanilla game exe
			LoadLibrary(L"wic_cl_hook.dll");
		}
		else
		{
			MessageBox(nullptr, L"Unknown game executable detected. DLL not loaded.", L"Injector error", MB_ICONERROR);
		}
	}

	Detours::IATHook((PBYTE)g_ModuleHandle, "WS2_32.dll", "gethostbyname", (PBYTE)&hk_gethostbyname);
	Detours::IATHook((PBYTE)g_ModuleHandle, "KERNEL32.dll", "GetSystemInfo", (PBYTE)&hk_GetSystemInfo);
	Detours::IATHook((PBYTE)g_ModuleHandle, "KERNEL32.dll", "SetThreadAffinityMask", (PBYTE)&hk_SetThreadAffinityMask);

	// Unhook and return to the original function prologue
	Detours::X86::DetourRemove(detourInfo);
	return ((int(__cdecl *)())detourEntry)();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	// Force this dll to be loaded permanently
	HMODULE temp;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_PIN | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPWSTR)hModule, &temp);

	// Determine the executable path
	g_ModuleHandle = GetModuleHandle(nullptr);

	if (!GetModuleFileName(g_ModuleHandle, g_ModulePath, ARRAYSIZE(g_ModulePath)))
		return FALSE;

	_wsplitpath_s(g_ModulePath, nullptr, 0, nullptr, 0, g_ExeName, ARRAYSIZE(g_ExeName), nullptr, 0);

	// Hook the original entrypoint
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)g_ModuleHandle;
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((uint8_t *)dosHeader + dosHeader->e_lfanew);
	uint8_t *entryAddress = ((uint8_t *)dosHeader + ntHeaders->OptionalHeader.AddressOfEntryPoint);

	detourInfo = Detours::X86::DetourFunction(entryAddress, (uint8_t *)&hk_mainCRTStartup);
	detourEntry = entryAddress;
	return TRUE;
}