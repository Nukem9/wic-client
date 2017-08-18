#include "stdafx.h"

wchar_t g_ModulePath[MAX_PATH];
HMODULE g_ModuleHandle;

void hk_mainCRTStartup()
{
	// Check which game exe this is (multiple variations, same version)
	if (wcsstr(g_ModulePath, L"wic.exe"))
	{
		// Vanilla game exe
		LoadLibrary(L"wic_cl_hook.dll");
	}
	else if (wcsstr(g_ModulePath, L"wic_online.exe"))
	{
		// Vanilla game exe, multiplayer-mode only
	}
	else if (wcsstr(g_ModulePath, L"wic_ds.exe"))
	{
		// Dedicated server
		LoadLibrary(L"wic_ds_hook.dll");
	}
	else if (wcsstr(g_ModulePath, L"wic-BroadcastTool.exe"))
	{
		// Broadcast (recording) tool
		LoadLibrary(L"wic_bt_hook.dll");
	}
	else if (wcsstr(g_ModulePath, L"WICEd.exe"))
	{
		// WIC map editor
	}
}

bool GetModuleVersion(wchar_t *Path, int *Major, int *Minor, int *Build, int *Revision)
{
	DWORD verHandle;
	DWORD verSize = GetFileVersionInfoSize(Path, &verHandle);

	if (verSize == 0)
		return false;

	// Allocate & read the version structures
	std::vector<BYTE> verData(verSize);
	{
		UINT size;
		LPVOID lpBuffer;

		if (!GetFileVersionInfo(Path, verHandle, verSize, verData.data()) || !VerQueryValue(verData.data(), L"\\", (LPVOID *)&lpBuffer, &size))
			return false;

		if (size < sizeof(VS_FIXEDFILEINFO))
			return false;

		// Check magic constant given by MSDN
		VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;

		if (verInfo->dwSignature != 0xFEEF04BD)
			return false;

		if (Major)
			*Major = (verInfo->dwFileVersionMS >> 16) & 0xffff;

		if (Minor)
			*Minor = (verInfo->dwFileVersionMS >> 0) & 0xffff;

		if (Build)
			*Build = (verInfo->dwFileVersionLS >> 16) & 0xffff;

		if (Revision)
			*Revision = (verInfo->dwFileVersionLS >> 0) & 0xffff;
	}

	return true;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

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
	PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)((size_t)dosHeader + dosHeader->e_lfanew);

	Detours::X86::DetourFunction((uint8_t *)ntHeaders->OptionalHeader.AddressOfEntryPoint, (uint8_t *)&hk_mainCRTStartup);

	DisableThreadLibraryCalls(hModule);
	return TRUE;
}