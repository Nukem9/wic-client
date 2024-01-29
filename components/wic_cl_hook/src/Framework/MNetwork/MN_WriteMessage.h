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
class MN_IWriteableDataStream;

class MN_WriteMessage : public MN_Message, public MN_SendFormat
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

public:
	MN_WriteMessage(u32 theTransportSize = MN_PACKET_DATA_SIZE);
	~MN_WriteMessage() = default;

	// Write routines for basic data types (can be extended)
	void WriteBool(bool aBool);
	void WriteChar(s8 aChar);
	void WriteUChar(u8 anUChar);
	void WriteShort(s16 aShort);
	void WriteUShort(u16 anUShort);
	void WriteInt(s32 anInt);
	void WriteUInt(u32 anUInt);
	void WriteInt64(s64 anInt);
	void WriteUInt64(u64 anUInt);
	void WriteFloat(float aFloat);
	void WriteString(const char* aString);
	void WriteLocString(const MC_LocString& aString);
	template<int S>
	void WriteLocString(const MC_StaticLocString<S>& aString) { WriteLocString(aString.GetBuffer(), aString.GetLength()); }
	void WriteLocString(const wchar_t* aString, u32 aStringLen);
	void WriteVector2f(const MC_Vector2f& aVector);
	void WriteVector3f(const MC_Vector3f& aVector);
	void WriteRawData(const void* someData, u16 aSize);

	// Optimized writes
	void WritePosition2f(const MC_Vector2f& aVector);	// Valid range [0 .. MN_POSITION_RANGE]
	void WritePosition3f(const MC_Vector3f& aVector);	// Valid range [0 .. MN_POSITION_RANGE]
	void WriteAngle(float aFloat);						// Valid range [-MN_ANGLE_RANGE .. MN_ANGLE_RANGE]
	void WriteUnitFloat(float aFloat);					// Valid range [0.0 .. 1.0]

	// Special, for debugging and network integrity
	void WriteDelimiter(const MN_DelimiterType& aCmd);

	// Bit vector
	void WriteBitVector(const MC_BitVector& aBitVector);

	// Append an existing MN_WriteMessage
	void Append(MN_WriteMessage& aMessage);

	// MN_SendFormat implementation
	MN_ConnectionErrorType SendMe(MN_IWriteableDataStream* aConnection, bool aDisableCompression = false) override;

private:
	// Append support
	void AppendEmpty(MN_WriteMessage& aMessage);
	void AppendNotEmpty(MN_WriteMessage& aMessage);
	bool PacketFitsInCurrentPacket(MN_Packet& aPacket);
	void CopyPacketToCurrentPacket(MN_Packet& aPacket);
	void AppendPacket(MN_Packet& aPacket);

	// Create a first packet for writing
	void CreateFirstWritePacket();

	// Shorten the current packet at the last NetCmd, stack it, and create a new packet for writing
	void WriteOverflow(u16 aLengthToWrite);

	// Creates first packet if needed and writes overflow if needed, writes typecheck identifier if ourTypeCheckFlag==true
	void PrepareWrite(u16 aLengthToWrite, s16 aTypeCheckValue);

	// Set internal flags like is compressed or is typechecked
	__forceinline void SetTypecheckFlag(u16& theField, bool aBoolean);
	__forceinline void SetCompressedFlag(u16& theField, bool aBoolean);

	u32 myDataPacketSize = 0;

	u8* GetDataBuffer(u32 theDataSize);
};
static_assert_size(MN_WriteMessage, 0x48);