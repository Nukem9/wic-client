#pragma once

class MN_Message
{
protected:
	CLASSPTR(volatile bool, 0x00E1F4DE, ourZipFlag);
	CLASSPTR(volatile bool, 0x00E1F4DF, ourCompressionFlag);
	CLASSPTR(volatile bool, 0x00DFDA6A, ourDefaultTypeCheckFlag);

public:
	virtual void *__vecDelDtor(unsigned int);

	bool							ourTypeCheckFlag;			// this+0x04
	MN_Packet						*myCurrentPacket;			// this+0x08
	MC_HybridArray<MN_Packet *, 8>	myPendingPackets;			// this+0x0C
	ushort							myLastDelimiter;			// this+0x38
	ushort							myLastDelimiterWriteOffset;	// this+0x3A
	uint							myPendingPacketMessageSize;	// this+0x3C

	//MN_Message();

	bool Empty();
};

CHECK_SIZE(MN_Message, 0x40);

class MN_MessageOnlineExeDisc
{
protected:
	CLASSPTR(volatile bool, 0x00DB8A0A, ourZipFlag);
	CLASSPTR(volatile bool, 0x00DB8A0B, ourCompressionFlag);
	CLASSPTR(volatile bool, 0x00D979BA, ourDefaultTypeCheckFlag);

public:
	virtual void *__vecDelDtor(unsigned int);

	bool							ourTypeCheckFlag;			// this+0x04
	MN_PacketOnlineExeDisc			*myCurrentPacket;			// this+0x08
	MC_HybridArray<MN_PacketOnlineExeDisc *, 8>	myPendingPackets;			// this+0x0C
	ushort							myLastDelimiter;			// this+0x38
	ushort							myLastDelimiterWriteOffset;	// this+0x3A
	uint							myPendingPacketMessageSize;	// this+0x3C

	//MN_Message();

	bool Empty();
};

CHECK_SIZE(MN_MessageOnlineExeDisc, 0x40);