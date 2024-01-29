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
#include <MCommon2/MC_String.h>
#include <MCommon2/MC_Vector.h>
#include "MN_Format.h"
#include "MN_Message.h"
#include "MN_Packet.h"

class MC_BitVector;

class MN_ReadMessage : public MN_Message, public MN_ReceiveFormat
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

public:
	MN_ReadMessage(u32 theTransportSize = MN_PACKET_DATA_SIZE);
	~MN_ReadMessage();

	bool SetLightTypecheckFlag(bool aFlag);

	bool ReadBool(bool& aBool);
	bool ReadChar(s8& aChar);
	bool ReadUChar(u8& anUChar);
	bool ReadShort(s16& aShort);
	bool ReadUShort(u16& anUShort);
	bool ReadInt(s32& anInt);
	bool ReadUInt(u32& anUInt);
	bool ReadInt64(s64& anInt);
	bool ReadUInt64(u64& anUInt);
	bool ReadFloat(float& aFloat);
	bool ReadString(MC_String& aString);
	bool ReadString(char* aBuffer, u32 aBufferSize);
	bool ReadLocString(MC_LocString& aString);
	bool ReadLocString(wchar_t* aBuffer, u32 aMaxBufferLen);

	// MAKE SURE YOU HAVE ROOM FOR THE DATA!
	bool ReadRawData(void* someData, s32 theMaxLength = -1, s32* outNumReadBytes = nullptr); // -1 means we can take anything in our buffer

	// Optimized reads
	bool ReadPosition2f(MC_Vector2f& aVector);	// Valid range [0 .. MN_POSITION_RANGE]
	bool ReadPosition3f(MC_Vector3f& aVector);	// Valid range [0 .. MN_POSITION_RANGE]
	bool ReadAngle(float& aFloat);				// [-MN_ANGLE_RANGE .. MN_ANGLE_RANGE]
	bool ReadUnitFloat(float& aFloat);			// Valid range [0.0 .. 1.0]

	// Returns the size of the string to be read. Not needed to read string, but
	// useful if you want to allocate memory yourself or want to check your current structure.
	// NOTE: After a call to ReadStringSize(), you HAVE to call ReadString() as well (unless
	// ReadStringSize returns 0).
	u16 ReadStringSize();

	// Reads vecotors from the network packet.
	bool ReadVector2f(MC_Vector2f& aVector);
	bool ReadVector3f(MC_Vector3f& aVector);

	// Special, for debugging and network integrity. Otherwise equivalent to ReadUChar().
	bool ReadDelimiter(MN_DelimiterType& aDelimiter);

	// Bit vector
	bool ReadBitVector(MC_BitVector& aBitVector);

	bool AtEnd() override;

	// MN_ReceiveFormat implementation
	// Should return the number of bytes used by the subclass implementation, 0 if none used
	u32 BuildMe(const void* aBuffer, const u32 aBufferLength) override;

private:
	MN_ReadMessage& operator=(const MN_ReadMessage&) = delete;

	// Packet operations
	u32 ConstructPackets(const u8* someData, const u32 aDataLength);
	void AddPacket(MN_Packet* aPacket);

	// Get next packet for reading. If fails, no more pending packets.
	bool NextReadPacket();

	// Returns true if typecheck succeeds or if typecheck is disabled
	bool TypeCheck(s16 aTypeCheckValue);

	// Returns true if typecheck succeeds and data is read
	bool ReadWithTypeCheck(s16 aTypeCheckValue, void *aBuffer, u16 aBufferSize);

	// Read offset in current packet
	u16 myReadOffset = 0;

	// Size of the string being read from the packet
	u16 myPendingStringSize = 0;

	u8*	ourUncompressedData = nullptr;
	u32 myDataPacketSize = 0;
	bool myLightTypecheckFlag = false;

	u8* GetDataBuffer(u32 theDataSize);
};