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

#include <type_traits>

// Abstract
class MMG_BitStream
{
public:
	enum StreamStatus
	{
		NoChange,
		OperationOk,
		EndOfStream
	};
	
	enum StreamOffset
	{
		StartPosition,
		CurrentPosition,
		EndPosition
	};

	u32 Tell()
	{
		return myPosition;
	}

	void Seek(StreamOffset theStart, u32 numBits)
	{
		switch (theStart)
		{
		case StartPosition:
			myPosition = 0;
		case CurrentPosition:
			myPosition += numBits;
			break;

		default:
			MC_ASSERT(false);
			break;
		}

		if (myPosition == myMaxLength)
			SetStatus(EndOfStream);
	}

	void Rewind()
	{
		myPosition = 0;
	}
	
	StreamStatus GetStatus()
	{
		return myStatus;
	}

protected:
	void SetStatus(StreamStatus theStatus)
	{
		myStatus = theStatus;
	}

	MMG_BitStream(u32 theMaxLengthInBits) : myMaxLength(theMaxLengthInBits)
	{
		myStatus = NoChange;
		myPosition = 0;
	}

	virtual ~MMG_BitStream() = 0
	{
	}

private:
	u32 myPosition;
	u32 myMaxLength;
	StreamStatus myStatus;
};

template<typename T>
requires(std::is_integral_v<T> && std::is_unsigned_v<T>)
class MMG_BitReader : public MMG_BitStream
{
public:
	MMG_BitReader(const T* theRawBuffer, u32 theRawBufferLengthInBits)
		: MMG_BitStream(theRawBufferLengthInBits)
		, myReadBuffer(theRawBuffer)
		, myBufferLength(theRawBufferLengthInBits)
	{
		// so we don't read outside by mistake (e.g. BitReader<int>(char* buffer, sizeof(char)))
		MC_ASSERT((theRawBufferLengthInBits / 8) >= sizeof(T));

		// make sure that bufferlen is even multiple T (e.g. BitReader<int>(int*, 40) is no good)
		MC_ASSERT((theRawBufferLengthInBits & (~(sizeof(T) * 8 - 1))) == theRawBufferLengthInBits);
	}

	virtual ~MMG_BitReader() = default;

	T ReadBit()
	{
		const T data = myReadBuffer[Tell() / (sizeof(T) * 8)];
		const T offset = 1 << (Tell() & (sizeof(T) * 8 - 1));

		Seek(CurrentPosition, 1);
		return (data & offset) > 0;
	}

	T ReadBits(u32 theNumBits)
	{
		if (theNumBits == 1)
			return ReadBit();

		MC_ASSERT(theNumBits <= sizeof(T) * 8);

		const u32 where = Tell();
		const u32 dataPos = where / (sizeof(T) * 8);
		const u32 shl = where & (sizeof(T) * 8 - 1);

		if (dataPos == ((where - 1 + theNumBits) / (sizeof(T) * 8)))
		{
			const T bitmask = ((1 << theNumBits) - 1) << shl;
			Seek(CurrentPosition, theNumBits);

			return ((myReadBuffer[dataPos] & bitmask) >> shl);
		}
		else // split the request into two recursive calls
		{
			const u32 secondHalf = ((where + theNumBits) & (sizeof(T) * 8 - 1));
			const u32 firstHalf = theNumBits - secondHalf;

			T retval = ReadBits(firstHalf);
			retval |= (ReadBits(secondHalf) << firstHalf);
			return retval;
		}

		return 0;
	}

private:
	MMG_BitReader() : MMG_BitStream(0)
	{
	}

	u32 myBufferLength;
	const T* myReadBuffer;
};

template<typename T>
requires(std::is_integral_v<T> && std::is_unsigned_v<T>)
class MMG_BitWriter : public MMG_BitStream
{
public:
	MMG_BitWriter(T* theDest, u32 theDestBufferLengthInBits)
		: MMG_BitStream(theDestBufferLengthInBits)
		, myDestBuffer(theDest)
		, myBufferLength(theDestBufferLengthInBits)
	{
		// so we don't write outside by mistake (e.g. BitWriter<int>(char* buffer, sizeof(char)))
		MC_ASSERT((theDestBufferLengthInBits / 8) >= sizeof(T));

		// make sure that bufferlen is even multiple T (e.g. BitWriter<int>(int*, 40) is no good)
		MC_ASSERT((theDestBufferLengthInBits & (~(sizeof(T) * 8 - 1))) == theDestBufferLengthInBits);
	}

	virtual ~MMG_BitWriter() = default;

	void WriteBit(T theBit)
	{
		const u32 where = Tell();
		const u32 dataPos = where / (sizeof(T) * 8);
		const T offset = 1 << (where & (sizeof(T) * 8 - 1));

		if (theBit)
			myDestBuffer[dataPos] |= offset;
		else
			myDestBuffer[dataPos] &= ~offset;

		Seek(CurrentPosition, 1);
	}

	void WriteBits(T theValue, u32 theNumBits)
	{
		if (theNumBits == 1)
			return WriteBit(theValue & 1);

		const u32 where = Tell();
		const u32 dataPos = where / (sizeof(T) * 8);

		if (dataPos == ((where - 1 + theNumBits) / (sizeof(T) * 8)))
		{
			const u32 shl = where & (sizeof(T) * 8 - 1);
			const T bitmask = ((1 << theNumBits) - 1) << shl;

			T& dest = myDestBuffer[dataPos];
			dest &= ~bitmask;
			dest |= (theValue << shl);

			Seek(CurrentPosition, theNumBits);
		}
		else
		{
			const u32 secondHalf = ((where + theNumBits) & (sizeof(T) * 8 - 1));
			const u32 firstHalf = theNumBits - secondHalf;

			WriteBits(theValue, firstHalf);
			WriteBits(theValue >> firstHalf, secondHalf);
		}
	}

private:
	MMG_BitWriter() : MMG_BitStream(0)
	{
	}

	u32 myBufferLength;
	T* myDestBuffer;
};