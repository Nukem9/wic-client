#include "../stdafx.h"

void MN_WriteMessage::PrepareWrite(uint16_t aLengthToWrite, short aTypeCheckValue)
{
	static DWORD ptr_MN_WriteMessage_PrepareWrite = 0x006B8C50;

	__asm
	{
		movzx eax, aTypeCheckValue
		push eax					// 3: aTypeCheckValue

		movzx eax, aLengthToWrite
		push eax					// 2: aLengthToWrite

		push ecx
		pop eax						// 1: thisptr

		call dword ptr ds:[ptr_MN_WriteMessage_PrepareWrite]
	}
}

void MN_WriteMessage::WriteBool(bool aBool)
{
	this->PrepareWrite(sizeof(bool), 'BL');
	this->myCurrentPacket->AppendData(sizeof(bool), &aBool);
}

void MN_WriteMessage::WriteChar(char aChar)
{
	this->PrepareWrite(sizeof(char), 'CH');
	this->myCurrentPacket->AppendData(sizeof(char), &aChar);
}

void MN_WriteMessage::WriteUChar(uint8_t aUChar)
{
	this->PrepareWrite(sizeof(uint8_t), 'UC');
	this->myCurrentPacket->AppendData(sizeof(uint8_t), &aUChar);
}

void MN_WriteMessage::WriteShort(short aShort)
{
	this->PrepareWrite(sizeof(short), 'SH');
	this->myCurrentPacket->AppendData(sizeof(short), &aShort);
}

void MN_WriteMessage::WriteUShort(uint16_t aUShort)
{
	this->PrepareWrite(sizeof(uint16_t), 'US');
	this->myCurrentPacket->AppendData(sizeof(uint16_t), &aUShort);
}

void MN_WriteMessage::WriteInt(int aInt)
{
	this->PrepareWrite(sizeof(int), 'IN');
	this->myCurrentPacket->AppendData(sizeof(int), &aInt);
}

void MN_WriteMessage::WriteUInt(uint32_t aUInt)
{
	this->PrepareWrite(sizeof(uint32_t), 'UI');
	this->myCurrentPacket->AppendData(sizeof(uint32_t), &aUInt);
}

void MN_WriteMessage::WriteInt64(__int64 aInt64)
{
	this->PrepareWrite(sizeof(__int64), 'I6');
	this->myCurrentPacket->AppendData(sizeof(__int64), &aInt64);
}

void MN_WriteMessage::WriteUInt64(uint64_t aUInt64)
{
	this->PrepareWrite(sizeof(uint64_t), 'U6');
	this->myCurrentPacket->AppendData(sizeof(uint64_t), &aUInt64);
}

void MN_WriteMessage::WriteFloat(float aFloat)
{
	this->PrepareWrite(sizeof(float), 'FL');
	this->myCurrentPacket->AppendData(sizeof(float), &aFloat);
}