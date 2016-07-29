#include "../stdafx.h"

static bool startup_flag=false;

namespace WIC_EXE
{
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

	HINSTANCE PASCAL hk_shellexecutea(HWND hwnd, LPCTSTR lpOperation, LPCTSTR lpFile, LPCTSTR lpParameters, LPCTSTR lpDirectory, INT nShowCmd)
	{
		if(strstr(lpFile, "www.massgate.net"))
		{
			int bufLength = strlen(lpFile);

			char *oldURL = "www.massgate.net";
			char *newURL = "http://127.0.0.1";	//urls need to be the same length for this to work

			char *tmp = (char *)malloc(bufLength + 1);
			memset(tmp, 0, sizeof(tmp));
			memcpy(tmp, lpFile, bufLength + 1);

			size_t index	= strstr(lpFile, oldURL) - lpFile;

			if(strstr(lpFile, oldURL))
			{
				for(int i = 0; i < strlen(newURL); i++)
					tmp[index+i] = newURL[i];
			}

			memcpy((void*)lpFile, tmp, bufLength + 1);
			free(tmp);
		}

		return ShellExecuteA(hwnd, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
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
	
	void InitializeHook()
	{
		DWORD d;

		// Hook gethostbyname (IAT)
		VirtualProtect((LPVOID)0x00BEC594, 4, PAGE_EXECUTE_READWRITE, &d);
		*(DWORD *)0x00BEC594 = (DWORD)&hk_gethostbyname;

		//hook ShellExecuteA
		VirtualProtect((LPVOID)0x00BEC3EC, 4, PAGE_EXECUTE_READWRITE, &d);
		*(DWORD *)0x00BEC3EC = (DWORD)&hk_shellexecutea;

		Detours::X86::DetourFunction((PBYTE)0x00A01520, (PBYTE)&WIC_WriteConsole);
		Detours::X86::DetourFunction((PBYTE)0x00BD1C73, (PBYTE)&hook);

		MMG_Protocols::MassgateProtocolVersion = 150;

		// Always enable multiple game instances
		VirtualProtect((LPVOID)0x00B2EFA9, 1, PAGE_EXECUTE_READWRITE, &d);
		*(BYTE *)0x00B2EFA9 = 0xEB;
	}
}