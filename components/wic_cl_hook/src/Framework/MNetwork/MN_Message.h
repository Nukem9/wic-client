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

// Compressed data for network transport

#include <Hooks/Memory.h>
#include <MCommon2/MC_HybridArray.h>

using MN_DelimiterType = u16;
class MN_Packet;

// Effective range for positions is [0 .. MN_POSITION_RANGE]
#define MN_POSITION_RANGE (1536.0f)

// Effective range for angles is [-MN_ANGLE_RANGE .. MN_ANGLE_RANGE]
#define MN_ANGLE_RANGE (8.0f)

// Message class
class MN_Message
{
public:
	static void EnableZipCompression(bool aState);
	static void EnableCompression(bool aState);
	static void EnableTypeChecking(bool aState);
	static bool ShouldZip();
	static bool IsCompressed();
	static bool IsTypeChecked();

	// Destructor
	virtual ~MN_Message();

	// Clear the message, and reset the data pointers
	void Clear();
	bool Empty();

	// Returns total size of all packets in message
	u32 GetMessageSize();
	MN_DelimiterType GetLastDelimiter();

protected:
	MN_Message();
	MN_Message& operator=(const MN_Message&) = delete;

	// Static members
	static inline DeclareGlobalRef(volatile bool, ourZipFlag, SelectOffset(0x00E1F4DE, 0x008EEF24));
	static inline DeclareGlobalRef(volatile bool, ourCompressionFlag, SelectOffset(0x00E1F4DF, 0x008EEF25));
	static inline DeclareGlobalRef(volatile bool, ourDefaultTypeCheckFlag, SelectOffset(0x00DFDA6A, 0x00871858));

	bool ourTypeCheckFlag = false;

	// Current packet
	MN_Packet *myCurrentPacket = nullptr;

	// Pending message packets
	MC_HybridArray<MN_Packet *, 8> myPendingPackets;

	// Position of last delimiter
	MN_DelimiterType myLastDelimiter = 0;
	u16 myLastDelimiterWriteOffset = 0;

	u32 myPendingPacketMessageSize = 0;
};
static_assert_size(MN_Message, 0x40);