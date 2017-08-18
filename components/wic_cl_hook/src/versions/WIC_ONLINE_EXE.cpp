#include "../stdafx.h"

namespace WIC_ONLINE_EXE
{
	void WIC_WriteConsole(const char *aString, ...)
	{
		char buffer[2048];
		va_list va;

		va_start(va, aString);
		_vsnprintf_s(buffer, _TRUNCATE, aString, va);
		va_end(va);

		// EX3D_Console::StuffText
		if (*(voidptr_t *)0x00DADCE4)
			((void (__cdecl *)(voidptr_t, const char *, ...))0x00AC0630)(*(voidptr_t *)0x00DADCE4, buffer);

		printf("%s\n", buffer);
	}

	struct hostent *PASCAL hk_gethostbyname(const char *name)
	{
		WIC_WriteConsole("gethostbyname(%s)", name);

		if(strstr(name, "massgate.net"))
			name = "127.0.0.1";

		return gethostbyname(name);
	}

	HINSTANCE PASCAL hk_shellexecutea(HWND hwnd, LPCTSTR lpOperation, LPCTSTR lpFile, LPCTSTR lpParameters, LPCTSTR lpDirectory, INT nShowCmd)
	{
		WIC_WriteConsole("%s", lpFile);

		if(strstr(lpFile, "www.massgate.net"))
		{
			size_t index = (size_t)(strstr(lpFile, "www.massgate.net") - lpFile);

			if (index)
				memcpy((void*)(lpFile+index), "http://127.0.0.1", 16);
		}

		WIC_WriteConsole("%s", lpFile);

		return ShellExecuteA(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
	}

	void hook_wrapper(void *accountQuery, MN_WriteMessageOnlineExeDisc *aMessage)
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

			mov cl, byte ptr ds:[0D979BAh]
			xor eax, eax

			push 00B7008Bh
			retn
		}
	}

	void InitializeHook()
	{
		DWORD d;

		// Hook gethostbyname (IAT)
		VirtualProtect((LPVOID)0x00B8E594, 4, PAGE_EXECUTE_READWRITE, &d);
		*(DWORD *)0x00B8E594 = (DWORD)&hk_gethostbyname;

		//hook ShellExecuteA
		VirtualProtect((LPVOID)0x00B8E3EC, 4, PAGE_EXECUTE_READWRITE, &d);
		*(DWORD *)0x00B8E3EC = (DWORD)&hk_shellexecutea;

		Detours::X86::DetourFunction((PBYTE)0x009C3FE0, (PBYTE)&WIC_WriteConsole);
		Detours::X86::DetourFunction((PBYTE)0x00B70083, (PBYTE)&hook);

		MMG_ProtocolsOnlineExeDisc::MassgateProtocolVersion = 150;

		// Always enable multiple game instances
		VirtualProtect((LPVOID)0x00AF1169, 1, PAGE_EXECUTE_READWRITE, &d);
		*(BYTE *)0x00AF1169 = 0xEB;
	}
}