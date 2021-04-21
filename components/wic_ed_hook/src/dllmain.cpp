#include "stdafx.h"

void WicED_HookInit(HMODULE hModule)
{
	// Wrap in a SEH block in order to prevent bad accesses for unknown modules. Not likely to happen but I'll keep it safe.
	__try
	{
		if (_stricmp(reinterpret_cast<const char *>(0x0077363A), "Built by alexanders/ME525 at 10:20:52 on May  5 2008 with compiler version 1400.\n") != 0)
		{
			//
			// ShowboxN.exe: Kill calls to _clearfp(), _controlfp(), and _controlfp_s() to prevent Win10 floating point exceptions (CRT embedded)
			//
			PatchMemory(0x005A2B41, { 0x31, 0xC0, 0xC3 });
			PatchMemory(0x00619469, { 0x31, 0xC0, 0xC3 });
			PatchMemory(0x0061B950, { 0x31, 0xC0, 0xC3 });
		}
		else if (_stricmp(reinterpret_cast<const char *>(0x0071874A), "Built by fredriko/ME411 at 13:14:05 on Nov 29 2007 with compiler version 1400.\n") != 0)
		{
			//
			// GuiEditor.exe: Kill calls to _clearfp(), _controlfp(), and _controlfp_s() to prevent Win10 floating point exceptions (CRT embedded)
			//
			PatchMemory(0x0065D70D, { 0x31, 0xC0, 0xC3 });
			PatchMemory(0x0065DAE7, { 0x31, 0xC0, 0xC3 });
			PatchMemory(0x0066F933, { 0x31, 0xC0, 0xC3 });
		}
		else if (_stricmp(reinterpret_cast<const char *>(0x00ABA008), "Built by gregore/ME646 at 10:58:47 on Mar  5 2009 with compiler version 1400.\n") != 0)
		{
			//
			// WICEd.exe: Kill calls to _clearfp(), _controlfp(), and _controlfp_s() to prevent Win10 floating point exceptions
			//
			auto module = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
			auto returnZeroFunction = +[]() -> int
			{
				return 0;
			};

			Detours::IATHook(module, "MSVCR80.dll", "_clearfp", reinterpret_cast<uintptr_t>(returnZeroFunction));
			Detours::IATHook(module, "MSVCR80.dll", "_controlfp", reinterpret_cast<uintptr_t>(returnZeroFunction));
			Detours::IATHook(module, "MSVCR80.dll", "_controlfp_s", reinterpret_cast<uintptr_t>(returnZeroFunction));
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	WicED_HookInit(hModule);
	return TRUE;
}