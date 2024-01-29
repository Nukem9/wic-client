#include <Windows.h>

void WIC_HookInit(HMODULE Module);
void WICDS_HookInit(HMODULE Module);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
#if WIC_MP
		WIC_HookInit(hModule);
#elif WIC_DS
		WICDS_HookInit(hModule);
#else
#error Unimplemented
#endif
	}

	return TRUE;
}