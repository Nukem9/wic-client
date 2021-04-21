#include "../stdafx.h"

void MN_Packet::AppendData(uint32_t aDataLen, void *aData)
{
	static DWORD ptr_MN_Packet_AppendData = 0x009E03E0;

	__asm
	{
		push aData			// 3: aData
		mov edi, aDataLen	// 2: aDataLen
		mov eax, this		// 1: thisptr

		call dword ptr [ptr_MN_Packet_AppendData]
	}
}

void MN_Packet::Clear()
{
	this->myWriteOffset = 0;
}