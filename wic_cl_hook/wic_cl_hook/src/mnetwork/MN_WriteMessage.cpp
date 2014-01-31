#include "../stdafx.h"

void MN_WriteMessage::PrepareWrite(ushort aLengthToWrite, short aTypeCheckValue)
{
	static DWORD ptr_MN_WriteMessage_PrepareWrite = 0x006B8C50;

	__asm
	{
		push aTypeCheckValue	// 3: aTypeCheckValue
		push aLengthToWrite		// 2: aLengthToWrite
		mov eax, this			// 1: thisptr

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

void MN_WriteMessage::WriteUChar(uchar aUChar)
{
	this->PrepareWrite(sizeof(uchar), 'UC');
	this->myCurrentPacket->AppendData(sizeof(uchar), &aUChar);
}

void MN_WriteMessage::WriteShort(short aShort)
{
	this->PrepareWrite(sizeof(short), 'SH');
	this->myCurrentPacket->AppendData(sizeof(short), &aShort);
}

void MN_WriteMessage::WriteUShort(ushort aUShort)
{
	this->PrepareWrite(sizeof(ushort), 'US');
	this->myCurrentPacket->AppendData(sizeof(ushort), &aUShort);
}

void MN_WriteMessage::WriteInt(int aInt)
{
	this->PrepareWrite(sizeof(int), 'IN');
	this->myCurrentPacket->AppendData(sizeof(int), &aInt);
}

void MN_WriteMessage::WriteUInt(uint aUInt)
{
	this->PrepareWrite(sizeof(uint), 'UI');
	this->myCurrentPacket->AppendData(sizeof(uint), &aUInt);
}

void MN_WriteMessage::WriteInt64(__int64 aInt64)
{
	this->PrepareWrite(sizeof(__int64), 'I6');
	this->myCurrentPacket->AppendData(sizeof(__int64), &aInt64);
}

void MN_WriteMessage::WriteUInt64(uint64 aUInt64)
{
	this->PrepareWrite(sizeof(uint64), 'U6');
	this->myCurrentPacket->AppendData(sizeof(uint64), &aUInt64);
}

void MN_WriteMessage::WriteFloat(float aFloat)
{
	this->PrepareWrite(sizeof(float), 'FL');
	this->myCurrentPacket->AppendData(sizeof(float), &aFloat);
}