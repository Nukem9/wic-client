#include "stdafx.h"

wchar_t g_ModulePath[1024];
wchar_t g_ExeName[1024];
HMODULE g_ModuleHandle;

uintptr_t detourInfo;
uintptr_t detourEntry;

void WINAPI hk_GetSystemInfo(LPSYSTEM_INFO lpSystemInfo)
{
	GetSystemInfo(lpSystemInfo);

	// Restrict core count to 16 because of a hardcoded array in MT_Supervisor
	if (lpSystemInfo)
		lpSystemInfo->dwNumberOfProcessors = std::min<DWORD>(lpSystemInfo->dwNumberOfProcessors, 16);
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
		// Modkit executables
		LoadLibraryW(L"wic_ed_hook.dll");
	}
	else
	{
		if (!_wcsicmp(g_ExeName, L"wic_ds"))
		{
			// Dedicated server
			LoadLibraryW(L"wic_ds_hook.dll");
		}
		else if (!_wcsicmp(g_ExeName, L"wic-BroadcastTool"))
		{
			// Broadcast (recording) tool
			LoadLibraryW(L"wic_bt_hook.dll");
		}
		else if (!_wcsicmp(g_ExeName, L"wic_online"))
		{
			// Vanilla game exe, multiplayer mode only
			//
			// This exe is pointless, so relaunch wic.exe that has actual patches
			if (!RelaunchWicExecutable(L"wic", L"wic_online"))
				MessageBoxW(nullptr, L"Unable to launch 'wic.exe'", L"Injector error", MB_ICONERROR);

			ExitProcess(0);
		}
		else if (!_wcsicmp(g_ExeName, L"wic") || !_wcsicmp(g_ExeName, L"wic_host"))
		{
			// Vanilla game exe
			LoadLibraryW(L"wic_cl_hook.dll");
		}
		else
		{
			MessageBoxW(nullptr, L"Unknown game executable detected. DLL not loaded.", L"Injector error", MB_ICONERROR);
		}
	}

	Detours::IATHook(reinterpret_cast<uintptr_t>(g_ModuleHandle), "KERNEL32.dll", "GetSystemInfo", reinterpret_cast<uintptr_t>(&hk_GetSystemInfo));
	Detours::IATHook(reinterpret_cast<uintptr_t>(g_ModuleHandle), "KERNEL32.dll", "SetThreadAffinityMask", reinterpret_cast<uintptr_t>(&hk_SetThreadAffinityMask));

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
	GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_PIN | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPWSTR>(hModule), &temp);

	// Determine the executable path
	g_ModuleHandle = GetModuleHandleW(nullptr);

	if (!GetModuleFileNameW(g_ModuleHandle, g_ModulePath, std::size(g_ModulePath)))
		return FALSE;

	_wsplitpath_s(g_ModulePath, nullptr, 0, nullptr, 0, g_ExeName, std::size(g_ExeName), nullptr, 0);

	// Hook the original entrypoint
	auto dosHeader = reinterpret_cast<const PIMAGE_DOS_HEADER>(g_ModuleHandle);
	auto ntHeaders = reinterpret_cast<const PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t *>(dosHeader) + dosHeader->e_lfanew);
	auto entryAddress = (reinterpret_cast<uintptr_t>(dosHeader) + ntHeaders->OptionalHeader.AddressOfEntryPoint);

	detourInfo = Detours::X86::DetourFunction(entryAddress, reinterpret_cast<uintptr_t>(&hk_mainCRTStartup));
	detourEntry = entryAddress;
	return TRUE;
}