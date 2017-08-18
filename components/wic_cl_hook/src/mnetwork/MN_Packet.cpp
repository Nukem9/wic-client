#include "../stdafx.h"

void MN_Packet::AppendData(uint aDataLen, voidptr_t aData)
{
	static DWORD ptr_MN_Packet_AppendData = 0x009D6740;

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

void MN_PacketOnlineExeDisc::AppendData(uint aDataLen, voidptr_t aData)
{
	static DWORD ptr_MN_Packet_AppendData = 0x00999510;

	__asm
	{
		push aData			// 3: aData
		mov edi, aDataLen	// 2: aDataLen
		mov eax, this		// 1: thisptr

		call dword ptr [ptr_MN_Packet_AppendData]
	}
}

void MN_PacketOnlineExeDisc::Clear()
{
	this->myWriteOffset = 0;
}