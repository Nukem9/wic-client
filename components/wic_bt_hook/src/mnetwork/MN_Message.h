#pragma once

class MN_Message
{
public:
	bool							ourTypeCheckFlag;			// this+0x04
	MN_Packet						*myCurrentPacket;			// this+0x08
	MC_HybridArray<MN_Packet *, 8>	myPendingPackets;			// this+0x0C
	uint16_t						myLastDelimiter;			// this+0x38
	uint16_t						myLastDelimiterWriteOffset;	// this+0x3A
	uint32_t						myPendingPacketMessageSize;	// this+0x3C

	virtual ~MN_Message();

	MN_Message() = delete;
	bool Empty();
};

CHECK_SIZE(MN_Message, 0x40);