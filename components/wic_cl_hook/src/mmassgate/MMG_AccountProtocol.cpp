#include "../stdafx.h"

void MMG_AccountProtocol::InitializeHook()
{
	Detours::X86::DetourFunctionClass(reinterpret_cast<uint8_t *>(0x00BD1C73), &Query__ToStream_00BD1C73);
}

void MMG_AccountProtocol::WriteEncryptionKeys(void *accountQuery, MN_WriteMessage *aMessage)
{
	auto myCipher = *reinterpret_cast<const MMG_BlockTEA **>(reinterpret_cast<uintptr_t>(accountQuery) + 0x4EA);

	for (long key : myCipher->myEncryptionKey)
		aMessage->WriteUInt(key);
}

void __declspec(naked) MMG_AccountProtocol::Query__ToStream_00BD1C73()
{
	__asm
	{
		push esi
		push ebx
		call WriteEncryptionKeys
		add esp, 0x8

		mov cl, byte ptr ds:[0x0DFDA6A]
		xor eax, eax

		push 0x00BD1C7B
		retn
	}
}