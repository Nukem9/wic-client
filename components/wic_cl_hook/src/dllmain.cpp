#include "stdafx.h"

/*
* *******************
* Signature scan for EXG_Game::EXG_PlayerContainer::myPlayers
* \x8D\x8B\x28\x00\x01\x00
* x?xxxx
* *******************
*/

void WIC_WriteConsole(const char *aString, ...)
{
	char buffer[2048];
	va_list va;

	va_start(va, aString);
	_vsnprintf_s(buffer, _TRUNCATE, aString, va);
	va_end(va);

	// EX3D_Console::StuffText
	if (*(voidptr_t *)0x00E147A4)
		((void (__cdecl *)(voidptr_t, const char *, ...))0x00AFDB90)(*(voidptr_t *)0x00E147A4, buffer);

	printf("%s\n", buffer);
}

struct hostent *PASCAL hk_gethostbyname(const char *name)
{
	WIC_WriteConsole("gethostbyname(%s)", name);

	if(strstr(name, "massgate.net"))
		name = "127.0.0.1";

	return gethostbyname(name);
}

void hook_wrapper(void *accountQuery, MN_WriteMessage *aMessage)
{
	MMG_BlockTEA *myCipher = *(MMG_BlockTEA **)((DWORD)accountQuery + 0x4EA);

	for(int i = 0; i < ARRAYSIZE(myCipher->myEncryptionKey); i++)
		aMessage->WriteUInt(myCipher->myEncryptionKey[i]);
}

void __declspec(naked) hook()
{
	__asm
	{
		push esi
		push ebx
		call hook_wrapper
		add esp, 8h

		mov cl, byte ptr ds:[0DFDA6Ah]
		xor eax, eax

		push 00BD1C7Bh
		retn
	}
}


/*#include "../steam/steam_api.h"
#pragma comment(lib, "steam/steam_api.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	if (SteamAPI_RestartAppIfNecessary(0))
		return 1;

	if (!SteamAPI_Init())
	{
		printf("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed).\n");
		//return 1;
	}

	MessageBoxA(nullptr, "", "", 0);

	return ((int (WINAPI *)(HINSTANCE, HINSTANCE, LPSTR, int))0x00B2EF40)(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
}
*/
BOOL Wic_HookInit(HMODULE hModule, DWORD ul_reason_for_call)
{
	DWORD d;

	//Detours::X86::DetourFunction((PBYTE)0x00554295, (PBYTE)&WinMain, Detours::X86Option::USE_CALL);

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	// Hook gethostbyname (IAT)
	VirtualProtect((LPVOID)0x00BEC594, 4, PAGE_EXECUTE_READWRITE, &d);
	*(DWORD *)0x00BEC594 = (DWORD)&hk_gethostbyname;

	//char buf[64];
	//sprintf_s(buf, "LIST: 0x%X", &addrList);
	//MessageBoxA(nullptr, buf, "", 0);

	Detours::X86::DetourFunction((PBYTE)0x00A01520, (PBYTE)&WIC_WriteConsole);
	Detours::X86::DetourFunction((PBYTE)0x00BD1C73, (PBYTE)&hook);

	//orig = (DWORD)Detours::X86::DetourFunction((PBYTE)0x00754F40, (PBYTE)&hk_EXG_Game_EXG_Game);

	MMG_CdKey::InitializeHook();
	MR_RenderD3D10::InitializeHook();

	//VirtualProtect((LPVOID)0x009356C8, 4, PAGE_EXECUTE_READWRITE, &d);
	//*(WORD *)0x009356C8 = 0x9090; LOS hack?

	// Set the protocol version
	MMG_Protocols::MassgateProtocolVersion = 150;

	// Always enable multiple game instances
	VirtualProtect((LPVOID)0x00B2EFA9, 1, PAGE_EXECUTE_READWRITE, &d);
	*(BYTE *)0x00B2EFA9 = 0xEB;

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	DisableThreadLibraryCalls(hModule);

	return Wic_HookInit(hModule, ul_reason_for_call);
}