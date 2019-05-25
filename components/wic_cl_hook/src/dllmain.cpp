#include "stdafx.h"

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

BOOL Wic_HookInit(HMODULE hModule, DWORD ul_reason_for_call)
{
#if 0
	if (AllocConsole())
	{
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
#endif

	// Set the protocol version
	MMG_Protocols::MassgateProtocolVersion = 150;

	// Always enable the console
	PatchMemory(0x00B31A16, (PBYTE)"\xEB", 1);

	// Copy MC_Debug::DebugMessage strings directly to the console output
	Detours::X86::DetourFunction((PBYTE)0x00A01520, (PBYTE)&WIC_WriteConsole);

	// Write MMG_AccountProtocol cipher keys directly after EncryptionKeySequenceNumber in message packets
	Detours::X86::DetourFunction((PBYTE)0x00BD1C73, (PBYTE)&hook);

	MN_NetRequester::InitializeHook();
	MMG_CdKey::InitializeHook();
	MR_RenderD3D10::InitializeHook();

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	return Wic_HookInit(hModule, ul_reason_for_call);
}