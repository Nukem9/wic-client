#pragma once

class MN_Packet
{
public:
	virtual void *__vecDelDtor(unsigned int);

	uint8_t		*myRawDataBuffer;	// this+0x04
	uint32_t	myRawDataBufferLen;	// this+0x08
	uint16_t&	myWriteOffset;		// this+0x0C
	uint16_t		myTransportSize;	// this+0x10

	void AppendData	(uint32_t aDataLen, void *aData);
	void Clear		();
};

CHECK_SIZE(MN_Packet, 0x14);