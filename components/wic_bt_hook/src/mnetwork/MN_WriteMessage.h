#pragma once

class MN_WriteMessage : public MN_Message, MN_SendFormat
{
public:
	uint32_t myDataPacketSize;// this+0x44

	void PrepareWrite(uint16_t aLengthToWrite, short aTypeCheckValue);

	void WriteBool(bool aBool);
	void WriteChar(char aChar);
	void WriteUChar(uint8_t aUChar);
	void WriteShort(short aShort);
	void WriteUShort(uint16_t aUShort);
	void WriteInt(int aInt);
	void WriteUInt(uint32_t aUInt);
	void WriteInt64(__int64 aInt64);
	void WriteUInt64(uint64_t aUInt64);
	void WriteFloat(float aFloat);
};

CHECK_SIZE(MN_WriteMessage, 0x48);