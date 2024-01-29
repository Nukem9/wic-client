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
#include "MN_WriteMessage.h"

MN_WriteMessage::MN_WriteMessage(u32 theTransportSize)
	: MN_Message()
	, myDataPacketSize(theTransportSize)
{
}

void MN_WriteMessage::WriteBool(bool aBool)
{
	static_assert(sizeof(bool) == 1, "Bool expected to fit in 1 byte as per network protocol.");

	// Correct potential invalid boolean states from type casts
	aBool = aBool != false;

	PrepareWrite(sizeof(aBool), 'BL');
	myCurrentPacket->AppendData(&aBool, sizeof(aBool));
}

void MN_WriteMessage::WriteChar(s8 aChar)
{
	PrepareWrite(sizeof(aChar), 'CH');
	myCurrentPacket->AppendData(&aChar, sizeof(aChar));
}

void MN_WriteMessage::WriteUChar(u8 aUChar)
{
	PrepareWrite(sizeof(aUChar), 'UC');
	myCurrentPacket->AppendData(&aUChar, sizeof(aUChar));
}

void MN_WriteMessage::WriteShort(s16 aShort)
{
	PrepareWrite(sizeof(aShort), 'SH');
	myCurrentPacket->AppendData(&aShort, sizeof(aShort));
}

void MN_WriteMessage::WriteUShort(u16 anUShort)
{
	PrepareWrite(sizeof(anUShort), 'US');
	myCurrentPacket->AppendData(&anUShort, sizeof(anUShort));
}

void MN_WriteMessage::WriteInt(s32 anInt)
{
	PrepareWrite(sizeof(anInt), 'IN');
	myCurrentPacket->AppendData(&anInt, sizeof(anInt));
}

void MN_WriteMessage::WriteUInt(u32 anUInt)
{
	PrepareWrite(sizeof(anUInt), 'UI');
	myCurrentPacket->AppendData(&anUInt, sizeof(anUInt));
}

void MN_WriteMessage::WriteInt64(s64 anInt)
{
	PrepareWrite(sizeof(anInt), 'I6');
	myCurrentPacket->AppendData(&anInt, sizeof(anInt));
}

void MN_WriteMessage::WriteUInt64(u64 anInt)
{
	PrepareWrite(sizeof(anInt), 'U6');
	myCurrentPacket->AppendData(&anInt, sizeof(anInt));
}

void MN_WriteMessage::WriteFloat(float aFloat)
{
	// MC_ASSERT(MC_Misc::SanityCheckFloat(aFloat));

	PrepareWrite(sizeof(aFloat), 'FL');
	myCurrentPacket->AppendData(&aFloat, sizeof(aFloat));
}

void MN_WriteMessage::WriteRawData(const void* someData, u16 aSize)
{
	PrepareWrite(sizeof(aSize) + aSize, 'RD');
	myCurrentPacket->AppendData(&aSize, sizeof(aSize));
	myCurrentPacket->AppendData(someData, aSize);
}

void MN_WriteMessage::WriteString(const char* aString)
{
	// If first write
	if (!myCurrentPacket)
		CreateFirstWritePacket();

	const auto length = static_cast<u16>(strlen(aString) + 1);

	// Handle potential overflow (length plus null character)
	if ((myCurrentPacket->GetWriteOffset() + sizeof(u16) + length /*+ sizeof(char)*/) >= myCurrentPacket->GetPacketDataCapacity())
		WriteOverflow(sizeof(u16) + length/* + sizeof(char)*/);

	// Write size of string so we don't have to search for it in ReadMessage
	myCurrentPacket->AppendData(&length, sizeof(length));
	myCurrentPacket->AppendData(aString, length);
}

void MN_WriteMessage::WriteLocString(const MC_LocString& aString)
{
	WriteLocString(aString, aString.GetLength());
}

void MN_WriteMessage::WriteLocString(const wchar_t* aString, u32 aStringLen)
{
	// If first write
	if (!myCurrentPacket)
		CreateFirstWritePacket();

	const auto length = static_cast<u16>(aStringLen + 1);

	// Handle potential overflow (length plus null character)
	if ((myCurrentPacket->GetWriteOffset() + sizeof(u16) + length * sizeof(MC_LocChar)) >= myCurrentPacket->GetPacketDataCapacity())
		WriteOverflow(sizeof(u16) + length * sizeof(MC_LocChar));

	// Write size of string so we don't have to search for it in ReadMessage
	myCurrentPacket->AppendData(&length, sizeof(length));
	myCurrentPacket->AppendData(aString, length * sizeof(MC_LocChar));
}

void MN_WriteMessage::WriteVector2f(const MC_Vector2f& aVector)
{
	PrepareWrite(sizeof(aVector), 'V2');
	myCurrentPacket->AppendData(&aVector, sizeof(aVector));
}

void MN_WriteMessage::WriteVector3f(const MC_Vector3f& aVector)
{
	PrepareWrite(sizeof(aVector), 'V3');
	myCurrentPacket->AppendData(&aVector, sizeof(aVector));
}

void MN_WriteMessage::WritePosition2f(const MC_Vector2f& aVector)
{
	// Valid range [0 .. MN_POSITION_RANGE]
	MC_ASSERT(aVector.x >= 0);
	MC_ASSERT(aVector.y >= 0);
	MC_ASSERT(aVector.x <= MN_POSITION_RANGE);
	MC_ASSERT(aVector.y <= MN_POSITION_RANGE);

	u16 s[2];
	s[0] = static_cast<u16>(aVector.x * (65535.99f / MN_POSITION_RANGE));
	s[1] = static_cast<u16>(aVector.y * (65535.99f / MN_POSITION_RANGE));

	PrepareWrite(sizeof(s), 'P2');
	myCurrentPacket->AppendData(s, sizeof(s));
}

void MN_WriteMessage::WritePosition3f(const MC_Vector3f& aVector)
{
	// Valid range [0 .. MN_POSITION_RANGE]
	MC_ASSERT(aVector.x >= 0);
	MC_ASSERT(aVector.y >= 0);
	MC_ASSERT(aVector.z >= 0);
	MC_ASSERT(aVector.x <= MN_POSITION_RANGE);
	MC_ASSERT(aVector.y <= MN_POSITION_RANGE);
	MC_ASSERT(aVector.z <= MN_POSITION_RANGE);

	u16 s[3];
	s[0] = static_cast<u16>(aVector.x * (65535.99f / MN_POSITION_RANGE));
	s[1] = static_cast<u16>(aVector.y * (65535.99f / MN_POSITION_RANGE));
	s[2] = static_cast<u16>(aVector.z * (65535.99f / MN_POSITION_RANGE));

	PrepareWrite(sizeof(s), 'P3');
	myCurrentPacket->AppendData(s, sizeof(s));
}

void MN_WriteMessage::WriteAngle(float aFloat)
{
	// Valid range [-MN_ANGLE_RANGE .. MN_ANGLE_RANGE]
	MC_ASSERT(aFloat > -MN_ANGLE_RANGE);
	MC_ASSERT(aFloat < MN_ANGLE_RANGE);

	const auto s = static_cast<u16>((aFloat + MN_ANGLE_RANGE) * (65535.99f / (MN_ANGLE_RANGE * 2)));

	PrepareWrite(sizeof(s), 'AN');
	myCurrentPacket->AppendData(&s, sizeof(s));
}

void MN_WriteMessage::WriteUnitFloat(float aFloat)
{
	// Valid range [0.0 .. 1.0]
	const auto t = static_cast<int>(aFloat * 65535.99f);
	const auto s = static_cast<u16>(std::clamp(t, 0, 65535));

	PrepareWrite(sizeof(s), 'UF');
	myCurrentPacket->AppendData(&s, sizeof(s));
}

void MN_WriteMessage::WriteDelimiter(const MN_DelimiterType& aDelimiter)
{
	// If first write
	if (!myCurrentPacket)
		CreateFirstWritePacket();

	// Store command and write offset
	myLastDelimiter = aDelimiter;
	myLastDelimiterWriteOffset = myCurrentPacket->GetWriteOffset();

	// Handle potential overflow
	if ((myCurrentPacket->GetWriteOffset() + sizeof(MN_DelimiterType) + (ourTypeCheckFlag ? sizeof(s16) : 0)) >= myCurrentPacket->GetPacketDataCapacity())
		WriteOverflow(sizeof(MN_DelimiterType) + (ourTypeCheckFlag ? sizeof(s16) : 0));

	if (ourTypeCheckFlag)
	{
		s16 typecheckId = 'DL';
		myCurrentPacket->AppendData(&typecheckId, sizeof(typecheckId));
	}

	myCurrentPacket->AppendData(&aDelimiter, sizeof(MN_DelimiterType));
}

/*
void MN_WriteMessage::WriteBitVector(const MC_BitVector& aBitVector)
{
	MC_ASSERT(aBitVector.GetDataLength() < MN_PACKET_DATA_SIZE - 32);	// Sanity check

	const u16 length = aBitVector.GetDataLength();

	PrepareWrite(sizeof(length) + length, 'BV');
	myCurrentPacket->AppendData(&length, sizeof(length));
	myCurrentPacket->AppendData(aBitVector.GetData(), length);
}

void MN_WriteMessage::Append(MN_WriteMessage& aMessage)
{
	// Don't support empty messages
	MC_ASSERT(!aMessage.Empty());

	// If I am empty, then can 
	if (Empty())
		AppendEmpty(aMessage);
	else
		AppendNotEmpty(aMessage);

	// At this point, we MUST have a valid myCurrentPacket, as we don't support appending empty messages
	// (first assert in this function)
	MC_ASSERT(myCurrentPacket);
}
*/

MN_ConnectionErrorType MN_WriteMessage::SendMe(MN_IWriteableDataStream* aConnection, bool aDisableCompression)
{
#if WIC_MP
	return ((MN_ConnectionErrorType(__thiscall *)(MN_SendFormat *, MN_IWriteableDataStream *, bool))0x006B8510)(this, aConnection, aDisableCompression);
#elif WIC_DS
	return ((MN_ConnectionErrorType(__thiscall *)(MN_SendFormat *, MN_IWriteableDataStream *, bool))0x006589D0)(this, aConnection, aDisableCompression);
#else
#error Unimplemented
#endif
}

void MN_WriteMessage::CreateFirstWritePacket()
{
	MC_ASSERT(!myCurrentPacket);

	myCurrentPacket = MN_Packet::Create(myDataPacketSize);
	MC_ASSERT(myCurrentPacket);
}

void MN_WriteMessage::WriteOverflow(u16 aLengthToWrite)
{
#if WIC_MP
	const static u32 dwCall = 0x006B88B0;

	__asm
	{
		mov esi, this
		push [aLengthToWrite]
		call [dwCall]
	}
#elif WIC_DS
	((void(__thiscall *)(MN_WriteMessage *, u16))0x006587B0)(this, aLengthToWrite);
#else
#error Unimplemented
#endif
}

void MN_WriteMessage::PrepareWrite(u16 aLengthToWrite, s16 aTypeCheckValue)
{
	// If first write
	if (!myCurrentPacket)
		CreateFirstWritePacket();

	// Handle potential overflow
	u16 numBytesNeeded = aLengthToWrite + (ourTypeCheckFlag ? sizeof(s16) : 0);

	if (myCurrentPacket->GetWriteOffset() + numBytesNeeded >= myCurrentPacket->GetPacketDataCapacity())
		WriteOverflow(numBytesNeeded);

	if (ourTypeCheckFlag)
		myCurrentPacket->AppendData(&aTypeCheckValue, sizeof(aTypeCheckValue));
}