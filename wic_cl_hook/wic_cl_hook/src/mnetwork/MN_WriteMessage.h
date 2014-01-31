#pragma once

class MN_WriteMessage : public MN_Message, MN_SendFormat
{
public:
	uint myDataPacketSize;// this+0x44

	void PrepareWrite(ushort aLengthToWrite, short aTypeCheckValue);

	void WriteBool		(bool aBool);
	void WriteChar		(char aChar);
	void WriteUChar		(uchar aUChar);
	void WriteShort		(short aShort);
	void WriteUShort	(ushort aUShort);
	void WriteInt		(int aInt);
	void WriteUInt		(uint aUInt);
	void WriteInt64		(__int64 aInt64);
	void WriteUInt64	(uint64 aUInt64);
	void WriteFloat		(float aFloat);
};

CHECK_SIZE(MN_WriteMessage, 0x48);