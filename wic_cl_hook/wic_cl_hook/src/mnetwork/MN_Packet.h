#pragma once

class MN_Packet
{
public:
	virtual void *__vecDelDtor(unsigned int);

	uchar	*myRawDataBuffer;	// this+0x04
	uint	myRawDataBufferLen;	// this+0x08
	ushort&	myWriteOffset;		// this+0x0C
	ushort	myTransportSize;	// this+0x10

	void AppendData	(uint aDataLen, voidptr_t aData);
	void Clear		();
};

CHECK_SIZE(MN_Packet, 0x14);