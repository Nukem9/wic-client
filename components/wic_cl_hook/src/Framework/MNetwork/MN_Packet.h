// Massgate
// Copyright (C) 2017 Ubisoft Entertainment
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// THIS FILE HAS BEEN MODIFIED FOR USE IN A NON-UBISOFT PROJECT. IT IS NOT
// AN ORIGINAL COPY.
//
#pragma once

#include <MCommon2/MC_HeapImplementation.h>

const static int MN_PACKET_DATA_SIZE = 1364;

// Atomic network packet
//
// NOTE: This class is very tied to Message, in that the upper two bits of Size is protocol flags. Keep them in
// transmission but do not get fooled by "large" sizes.
class MN_Packet
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

private:
	u8*	myRawDataBuffer;
	u32 myRawDataBufferLen;

	// Offset for current write position in data
	u16& myWriteOffset;
	u16 myTransportSize;

public:
	/// Create a UDP packet with default MTU.
	static MN_Packet*		Create(u32 theTransportSize = 512);

	// Create a new packet from an existing packet. Will also get the same MTU.
	static MN_Packet*		Create(const MN_Packet* aPacket);

	// Deletes the packet. The calling pointer should be invalidated immediately after calling this.
	static void				Deallocate(MN_Packet*& aPacket);

	// Call this to deallocate the statically allocated packets.
	static void				CleanUp();

	// Size of packet including headers and data, i.e. all that is necessary for sending on the network.
	u16						GetBinarySize()	const { return (sizeof(myWriteOffset) + GetWriteOffset() * sizeof(u8)); }

	// Returns the full data (including any headers) which is very suitable for sending on the network.
	const u8*				GetBinaryData() const { return myRawDataBuffer; }

	/// Assignment operator.
	void					operator=(const MN_Packet& aPacket);

	// Gets the write offset for new data.
	const u16				GetWriteOffset() const { return myWriteOffset & 0x3fff; }

	/// Sets the write offset for new data.
	void					SetWriteOffset(u16 aWo) { MC_ASSERT((aWo < myRawDataBufferLen) && (aWo >= 0)); myWriteOffset = aWo; }

	// Gets the data in the packet EXCLUDING any headers. Don't send this over the network.
	const u8*				GetData() const { MC_ASSERT(myRawDataBuffer); return myRawDataBuffer + sizeof(s16); }

	// Appends data to the packet.
	void					AppendData(const void* someData, u32 someDatalen);

	// Clears the contents of the packet.
	void					Clear();

	/// Returns the maximum amount of data that can be written to the packet.
	u16						GetPacketDataCapacity() const { return myTransportSize - sizeof(s16); }

private:
	MN_Packet();
	MN_Packet(u8* anAllocatedBuffPtr, u32 theTransportSize);
	explicit MN_Packet(const MN_Packet& aPacket);
	virtual ~MN_Packet();

	static MN_Packet* GetNewPacket(u32 theTransportSize);
};
static_assert_size(MN_Packet, 0x14);