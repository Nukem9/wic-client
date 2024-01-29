#include "../stdafx.h"

void MMG_AccountProtocol::InitializeHook()
{
	Detours::X86::DetourFunctionT(0x00BE04F3, &Query__ToStream_00BE04F3);
}

void MMG_AccountProtocol::WriteEncryptionKeys(void *accountQuery, MN_WriteMessage *aMessage)
{
	auto myCipher = *reinterpret_cast<const MMG_BlockTEA **>(reinterpret_cast<uintptr_t>(accountQuery) + 0x4EA);

	for (long key : myCipher->myEncryptionKey)
		aMessage->WriteUInt(key);
}

void __declspec(naked) MMG_AccountProtocol::Query__ToStream_00BE04F3()
{
	__asm
	{
		push esi
		push ebx
		call WriteEncryptionKeys
		add esp, 0x8

		mov cl, byte ptr ds:[0x0E11DA4]
		xor eax, eax

		push 0x00BE04FB
		retn
	}
}