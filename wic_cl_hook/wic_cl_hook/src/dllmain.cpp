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

	if (!*(voidptr_t *)0x00E147A4)
		return;

	va_start(va, aString);
	_vsnprintf_s(buffer, _TRUNCATE, aString, va);
	va_end(va);

	// EX3D_Console::StuffText
	((void (__cdecl *)(voidptr_t, const char *, ...))0x00AFDB90)(*(voidptr_t *)0x00E147A4, buffer);
}

struct hostent *PASCAL hk_gethostbyname(const char *name)
{
	WIC_WriteConsole("gethostbyname(%s)", name);

	if(strstr(name, "massgate.net"))
		name = "127.0.0.1";

	return gethostbyname(name);
}

DWORD addrList[100];
int addrCount;


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

void SetBits(DWORD_PTR& dw, int lowBit, int bits, int newValue)
{
	DWORD_PTR mask = (1 << bits) - 1; 
	dw = (dw & ~(mask << lowBit)) | (newValue << lowBit);
}

void AddAddr(DWORD addr)
{
	for(int i = 0; i < addrCount; i++)
	{
		if(addrList[i] == addr)
			return;
	}

	addrList[addrCount++] = addr;
}

DWORD g_addr = 0;
LONG WINAPI VectoredHandler(PEXCEPTION_POINTERS ExceptionInfo)
{
	DWORD code	= (DWORD)ExceptionInfo->ExceptionRecord->ExceptionCode;
	DWORD addr	= (DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress;
	DWORD eip	= (DWORD)ExceptionInfo->ContextRecord->Eip;

	if(code == 0x406D1388)
		return EXCEPTION_CONTINUE_SEARCH;

	//char buf[64];
	//sprintf_s(buf, "CODE: 0x%X - 0x%X", code, addr);
	//MessageBoxA(nullptr, buf, "", 0);

	if(code == EXCEPTION_ACCESS_VIOLATION)
	{
		if(addr == 0x100)
		{
			WIC_WriteConsole("SET BREAKPOINT (0x%X)", ExceptionInfo->ContextRecord->Eax);

			g_addr = ExceptionInfo->ContextRecord->Eax;

			// Set hardware breakpoint
			ExceptionInfo->ContextRecord->Dr0 = ExceptionInfo->ContextRecord->Eax;
			//ExceptionInfo->ContextRecord->Dr7 = (1 << 0);

			SetBits(ExceptionInfo->ContextRecord->Dr7, 16, 2, 3);
			SetBits(ExceptionInfo->ContextRecord->Dr7, 18, 2, 3);
			SetBits(ExceptionInfo->ContextRecord->Dr7, 0, 1, 1);

			// Continue execution
			ExceptionInfo->ContextRecord->Eip = ExceptionInfo->ContextRecord->Edi;
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}

	if(code == EXCEPTION_SINGLE_STEP)
	{
		//if(ExceptionInfo->ExceptionRecord->ExceptionAddress == (PVOID)g_addr)
		//{
			WIC_WriteConsole("Addr: 0x%X", ExceptionInfo->ContextRecord->Eip);
			AddAddr(ExceptionInfo->ContextRecord->Eip);
			return EXCEPTION_CONTINUE_EXECUTION;
		//}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

// Gets the value of EAX
DWORD orig;
void __declspec(naked) hk_EXG_Game_EXG_Game()
{
	__asm
	{
		push eax
		push edx
		push edi

		// Set the HWBP address
		mov eax, dword ptr ss:[esp + 4h]
		add eax, 1007Ch

		// Set the jump back address
		mov edi, offset __jmpback

		// Jump to 0x100 (trigger access violation)
		mov edx, 100h
		jmp edx

	__jmpback:

		pop edi
		pop edx
		pop eax

		// Jump to EXG_Game::EXG_Game()
		jmp dword ptr ds:[orig]
	}
}

BOOL Wic_HookInit(HMODULE hModule, DWORD ul_reason_for_call)
{
	DWORD d;

	// Hook gethostbyname (IAT)
	//VirtualProtect((LPVOID)0x00BEC594, 4, PAGE_EXECUTE_READWRITE, &d);
	//*(DWORD *)0x00BEC594 = (DWORD)&hk_gethostbyname;

	char buf[64];
	sprintf_s(buf, "LIST: 0x%X", &addrList);
	MessageBoxA(nullptr, buf, "", 0);


	Detours::X86::DetourFunction((PBYTE)0x00A01520, (PBYTE)&WIC_WriteConsole);
	Detours::X86::DetourFunction((PBYTE)0x00BD1C73, (PBYTE)&hook);

	AddVectoredExceptionHandler(0, VectoredHandler);

	orig = (DWORD)Detours::X86::DetourFunction((PBYTE)0x00754F40, (PBYTE)&hk_EXG_Game_EXG_Game);

	MR_RenderD3D10::InitializeHook();

	VirtualProtect((LPVOID)0x009356C8, 4, PAGE_EXECUTE_READWRITE, &d);
	*(WORD *)0x009356C8 = 0x9090;

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