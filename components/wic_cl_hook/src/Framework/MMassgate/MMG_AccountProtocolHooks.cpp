#include <Hooks/Hooks.h>
#include <MNetwork/MN_WriteMessage.h>
#include "MMG_BlockTEA.h"
#include "MMG_AccountProtocol.h"

void MMG_AccountProtocol__Query__ToStream_00BD1C73();

//
// Write MMG_AccountProtocol cipher keys directly after EncryptionKeySequenceNumber in query packets
//
DECLARE_HOOK_INITIALIZER_MP(MMG_AccountProtocol,
{
	Hooks::WriteJump(0x00BD1C73, &MMG_AccountProtocol__Query__ToStream_00BD1C73);
});

void __declspec(naked) MMG_AccountProtocol__Query__ToStream_00BD1C73()
{
	MMG_AccountProtocol::Query *protocolQuery;
	MN_WriteMessage *outgoingMessage;

	__asm
	{
		pushad
		mov ebp, esp
		sub esp, __LOCAL_SIZE
		mov protocolQuery, ebx
		mov outgoingMessage, esi
	}

	s32 keys[4];
	static_cast<const MMG_BlockTEA *>(protocolQuery->myEncrypter)->GetKey(keys);

	for (s32 key : keys)
		outgoingMessage->WriteUInt(*reinterpret_cast<u32 *>(&key));

	__asm
	{
		mov esp, ebp
		popad

		mov cl, byte ptr ds:[0x0DFDA6A]
		xor eax, eax

		push 0x00BD1C7B
		retn
	}
}