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
#include "MN_ReadMessage.h"

static const u32 LOC_BUFFER_MULTIPLIER = 10;

MN_ReadMessage::MN_ReadMessage(u32 theTransportSize)
	: MN_Message(),
	myDataPacketSize(theTransportSize),
	ourUncompressedData(GetDataBuffer(theTransportSize * LOC_BUFFER_MULTIPLIER))
{
}

MN_ReadMessage::~MN_ReadMessage()
{
	for (auto& packet : myPendingPackets)
		MN_Packet::Deallocate(packet);

	myPendingPackets.clear();
}

bool MN_ReadMessage::SetLightTypecheckFlag(bool aFlag)
{
	bool oldState = myLightTypecheckFlag;
	myLightTypecheckFlag = aFlag;

	return oldState;
}

bool MN_ReadMessage::ReadBool(bool& aBool)
{
	char tempValue = 0;

	if (!ReadWithTypeCheck('BL', &tempValue, sizeof(tempValue)))
		return false;

	switch (tempValue)
	{
	case 0:
		aBool = false;
		return true;

	case 1:
		aBool = true;
		return true;
	}

	return false;
}

bool MN_ReadMessage::ReadChar(s8& aChar)
{
	return ReadWithTypeCheck('CH', &aChar, sizeof(aChar));
}

bool MN_ReadMessage::ReadUChar(u8& anUChar)
{
	return ReadWithTypeCheck('UC', &anUChar, sizeof(anUChar));
}

bool MN_ReadMessage::ReadShort(s16& aShort)
{
	return ReadWithTypeCheck('SH', &aShort, sizeof(aShort));
}

bool MN_ReadMessage::ReadUShort(u16& anUShort)
{
	return ReadWithTypeCheck('US', &anUShort, sizeof(anUShort));
}

bool MN_ReadMessage::ReadInt(s32& anInt)
{
	return ReadWithTypeCheck('IN', &anInt, sizeof(anInt));
}

bool MN_ReadMessage::ReadUInt(u32& anUInt)
{
	return ReadWithTypeCheck('UI', &anUInt, sizeof(anUInt));
}

bool MN_ReadMessage::ReadInt64(s64& anInt)
{
	return ReadWithTypeCheck('I6', &anInt, sizeof(anInt));
}

bool MN_ReadMessage::ReadUInt64(u64& anUInt)
{
	return ReadWithTypeCheck('U6', &anUInt, sizeof(anUInt));
}

bool MN_ReadMessage::ReadFloat(float& aFloat)
{
	if (!ReadWithTypeCheck('FL', &aFloat, sizeof(aFloat)))
		return false;

	//if (!MC_Misc::SanityCheckFloat(aFloat))
	//	return false;

	return true;
}

u16 MN_ReadMessage::ReadStringSize()
{
	// If first read
	if (!myCurrentPacket)
	{
		if (!NextReadPacket())
		{
			myPendingStringSize = 0;
			return 0;
		}
	}

	if ((myReadOffset + sizeof(u16)) <= myCurrentPacket->GetWriteOffset())
	{
		memcpy(&myPendingStringSize, myCurrentPacket->GetData() + myReadOffset, sizeof(u16));
		myReadOffset += sizeof(u16);
	}
	else
		myPendingStringSize = 0;

	return myPendingStringSize;
}

bool MN_ReadMessage::ReadString(MC_String& aString)
{
	if (myPendingStringSize == 0)
	{
		if (ReadStringSize() == 0)
		{
			aString = "";
			return false;
		}
	}

	aString = MC_String(myPendingStringSize, '\0');
	return ReadString(aString.GetBuffer(), aString.GetBufferSize());
}

bool MN_ReadMessage::ReadString(char* aBuffer, u32 aBufferSize)
{
	memset(aBuffer, 0, aBufferSize * sizeof(char));

	if (myPendingStringSize == 0)
		ReadStringSize();

	if ((myPendingStringSize == 0) || (myPendingStringSize > aBufferSize))
		return false;

	if ((myReadOffset + myPendingStringSize) > myCurrentPacket->GetWriteOffset())
		return false;

	memcpy(aBuffer, myCurrentPacket->GetData() + myReadOffset, myPendingStringSize);
	aBuffer[myPendingStringSize - 1] = '\0';

	myReadOffset += myPendingStringSize * sizeof(char);
	myPendingStringSize = 0;

	// Read next chunk of current packet at end
	if (myReadOffset >= myCurrentPacket->GetWriteOffset())
		NextReadPacket();

	return true;
}

bool MN_ReadMessage::ReadLocString(MC_LocString& aString)
{
	if (myPendingStringSize == 0)
	{
		if (ReadStringSize() == 0)
		{
			aString = L"";
			return false;
		}
	}

	aString = MC_LocString(myPendingStringSize, '\0');
	return ReadLocString(aString.GetBuffer(), aString.GetBufferSize());
}

bool MN_ReadMessage::ReadLocString(wchar_t* aBuffer, u32 aBufferSize)
{
	memset(aBuffer, 0, aBufferSize * sizeof(wchar_t));

	if (myPendingStringSize == 0)
		ReadStringSize();

	if ((myPendingStringSize == 0) || (myPendingStringSize > aBufferSize))
		return false;

	if ((myReadOffset + myPendingStringSize) > myCurrentPacket->GetWriteOffset())
		return false;

	memcpy(aBuffer, myCurrentPacket->GetData() + myReadOffset, myPendingStringSize * sizeof(wchar_t));
	aBuffer[myPendingStringSize - 1] = L'\0';

	myReadOffset += myPendingStringSize * sizeof(wchar_t);
	myPendingStringSize = 0;

	// Read next chunk of current packet at end
	if (myReadOffset >= myCurrentPacket->GetWriteOffset())
		NextReadPacket();

	return true;
}

bool MN_ReadMessage::ReadRawData(void* someData, s32 theMaxLen, s32* outNumReadBytes)
{
	u16 dataSize = 0;

	if (!ReadWithTypeCheck('RD', &dataSize, sizeof(dataSize)))
		return false;

	if ((theMaxLen != -1) && (dataSize > theMaxLen))
		return false;

	if ((myReadOffset + dataSize) > myCurrentPacket->GetWriteOffset())
		return false;

	if (someData)
		memcpy(someData, myCurrentPacket->GetData() + myReadOffset, dataSize);

	myReadOffset += dataSize;

	if (outNumReadBytes)
		*outNumReadBytes = dataSize;

	// Read next chunk of current packet at end
	if (myReadOffset >= myCurrentPacket->GetWriteOffset())
		NextReadPacket();

	return true;
}

bool MN_ReadMessage::ReadVector2f(MC_Vector2f& aVector)
{
	return ReadWithTypeCheck('V2', &aVector, sizeof(aVector));
}

bool MN_ReadMessage::ReadVector3f(MC_Vector3f& aVector)
{
	return ReadWithTypeCheck('V3', &aVector, sizeof(aVector));
}

bool MN_ReadMessage::ReadPosition2f(MC_Vector2f& aVector)
{
	u16 compressed[2];

	if (!ReadWithTypeCheck('P2', &compressed, sizeof(compressed)))
		return false;

	aVector.x = static_cast<float>(compressed[0]) * (MN_POSITION_RANGE / 65535.0f);
	aVector.y = static_cast<float>(compressed[1]) * (MN_POSITION_RANGE / 65535.0f);
	return true;
}

bool MN_ReadMessage::ReadPosition3f(MC_Vector3f& aVector)
{
	u16 compressed[3];

	if (!ReadWithTypeCheck('P3', &compressed, sizeof(compressed)))
		return false;

	aVector.x = static_cast<float>(compressed[0]) * (MN_POSITION_RANGE / 65535.0f);
	aVector.y = static_cast<float>(compressed[1]) * (MN_POSITION_RANGE / 65535.0f);
	aVector.z = static_cast<float>(compressed[2]) * (MN_POSITION_RANGE / 65535.0f);
	return true;
}

bool MN_ReadMessage::ReadAngle(float& aFloat)
{
	u16 compressed[1];

	if (!ReadWithTypeCheck('AN', &compressed, sizeof(compressed)))
		return false;

	aFloat = static_cast<float>(compressed[0]) * ((MN_ANGLE_RANGE * 2) / 65535.0f) - MN_ANGLE_RANGE;
	return true;
}

bool MN_ReadMessage::ReadUnitFloat(float& aFloat)
{
	u16 compressed[1];

	if (!ReadWithTypeCheck('UF', &compressed, sizeof(compressed)))
		return false;

	aFloat = static_cast<float>(compressed[0]) * (1.0f / 65535.0f);
	return true;
}

bool MN_ReadMessage::ReadDelimiter(MN_DelimiterType& aDelimiter)
{
	return ReadWithTypeCheck('DL', &aDelimiter, sizeof(aDelimiter));
}

/*
bool MN_ReadMessage::ReadBitVector(MC_BitVector& aBitVector)
{
	return ReadWithTypeCheck('BV', &aBitVector, sizeof(aBitVector));
}
*/

bool MN_ReadMessage::NextReadPacket()
{
#if WIC_MP
	const static u32 dwCall = 0x006B9560;

	__asm
	{
		mov edi, this
		call [dwCall]
	}
#elif WIC_DS
	return ((bool(__thiscall *)(MN_ReadMessage *))0x00657220)(this);
#else
#error Unimplemented
#endif
}

bool MN_ReadMessage::TypeCheck(s16 aTypeCheckValue)
{
	if (!ourTypeCheckFlag)
		return true;

	// If first read
	if (!myCurrentPacket)
	{
		if (!NextReadPacket())
			return true;
	}

	s16 readValue = 0;
	if ((myReadOffset + sizeof(readValue)) <= myCurrentPacket->GetWriteOffset())
	{
		memcpy(&readValue, myCurrentPacket->GetData() + myReadOffset, sizeof(readValue));
		myReadOffset += sizeof(readValue);
	}
	else
	{
		MC_DEBUG("Typecheck failed. Data type should have been %c%c but there was no data available. Last delimiter was %d",
			char(aTypeCheckValue >> 8), char(aTypeCheckValue),
			int(myLastDelimiter));

		// MODIFIED: We don't use these checks anymore
		//if (!myLightTypecheckFlag)
		//	MC_ASSERTMSG(false, "Typecheck failed, no data");

		return false;
	}

	// Handle potential overflow
	if (myReadOffset >= myCurrentPacket->GetWriteOffset())
		NextReadPacket();

	if (readValue != aTypeCheckValue)
	{
		MC_DEBUG("Typecheck failed. Data type was %c%c, should have been %c%c. Last delimiter was %d",
			char(readValue >> 8), char(readValue),
			char(aTypeCheckValue >> 8), char(aTypeCheckValue),
			int(myLastDelimiter));

		// MODIFIED: We don't use these checks anymore
		//if (!myLightTypecheckFlag)
		//	MC_ASSERTMSG(false, "Typecheck failed, wrong type");

		return false;
	}

	return true;
}

bool MN_ReadMessage::AtEnd()
{
	// Next chunk
	if (myCurrentPacket && !myPendingPackets.empty() && myReadOffset >= myCurrentPacket->GetWriteOffset())
		NextReadPacket();

	if (myCurrentPacket)
	{
		if (myReadOffset >= myCurrentPacket->GetWriteOffset())
			return true;
		
		return false;
	}

	return myPendingPackets.empty();
}

u32 MN_ReadMessage::BuildMe(const void* aBuffer, const u32 aBufferLength)
{
#if WIC_MP
	return ((u32(__thiscall *)(MN_ReceiveFormat *, const void *, const u32))0x006B97B0)(this, aBuffer, aBufferLength);
#elif WIC_DS
	return ((u32(__thiscall *)(MN_ReceiveFormat *, const void *, const u32))0x00658300)(this, aBuffer, aBufferLength);
#else
#error Unimplemented
#endif
}

bool MN_ReadMessage::ReadWithTypeCheck(s16 aTypeCheckValue, void *aBuffer, u16 aBufferSize)
{
	// If first read
	if (!myCurrentPacket)
	{
		if (!NextReadPacket())
			return false;
	}

	if (!TypeCheck(aTypeCheckValue))
		return false;

	bool status = false;
	if ((myReadOffset + aBufferSize) <= myCurrentPacket->GetWriteOffset())
	{
		memcpy(aBuffer, myCurrentPacket->GetData() + myReadOffset, aBufferSize);
		myReadOffset += aBufferSize;

		status = true;
	}

	// Handle potential overflow
	if (myReadOffset >= myCurrentPacket->GetWriteOffset())
		NextReadPacket();

	return status;
}

u8* MN_ReadMessage::GetDataBuffer(u32 theDataSize)
{
#if WIC_MP
	const static u32 dwCall = 0x006B94C0;

	__asm
	{
		mov edi, [theDataSize]
		call [dwCall]
	}
#elif WIC_DS
	return ((u8 *(__thiscall *)(MN_ReadMessage *, u32))0x00656F70)(this, theDataSize);
#else
#error Unimplemented
#endif
}