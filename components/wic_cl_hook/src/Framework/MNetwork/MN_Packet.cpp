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
#include <Hooks/Memory.h>
#include <MCommon2/MC_GrowingArray.h>
#include <MCommon2/MC_Debug.h>
#include <MCommon2/MT_Mutex.h>
#include <mutex>
#include <limits>
#include "MN_Packet.h"

static DeclareGlobalRef(MC_GrowingArray<MN_Packet *>, locPacketArray, SelectOffset(0x00E01938, 0x00871848));
static DeclareGlobalRef(MT_Mutex, locPacketAllocatorMutex, SelectOffset(0x00E01948, 0x008EEF04));

MT_Mutex& GetLocPacketAllocatorMutex()
{
	return locPacketAllocatorMutex;
}

MN_Packet* MN_Packet::Create(u32 theTransportSize)
{
	std::scoped_lock locker(GetLocPacketAllocatorMutex());

	auto newPacket = GetNewPacket(theTransportSize);
	MC_ASSERT(newPacket);

	newPacket->Clear();
	return newPacket;
}

MN_Packet* MN_Packet::Create(const MN_Packet* aPacket)
{
	std::scoped_lock locker(GetLocPacketAllocatorMutex());

	auto newPacket = GetNewPacket(aPacket->myRawDataBufferLen);
	MC_ASSERT(newPacket);
	MC_ASSERT(newPacket != aPacket);

	*newPacket = *aPacket;
	return newPacket;
}

void MN_Packet::Deallocate(MN_Packet*& aPacket)
{
	std::scoped_lock locker(GetLocPacketAllocatorMutex());

	for (auto& packet : locPacketArray)
		MC_ASSERT(packet != aPacket);

	if (locPacketArray.size() < 128)
		locPacketArray.push_back(aPacket);
	else
		delete aPacket;

	aPacket = nullptr;
}

/*
void MN_Packet::CleanUp()
{
	std::scoped_lock locker(GetLocPacketAllocatorMutex());

	for (int i = 0; i < locPacketArray.size(); i++)
	{
		delete locPacketArray[i];
		locPacketArray.RemoveCyclicAtIndex(i--);
	}
}
*/
void
MN_Packet::AppendData(const void* someData, u32 someDatalen)
{
	const u32 writeOffset = GetWriteOffset() + sizeof(s16);

	if ((writeOffset + someDatalen) > myRawDataBufferLen)
	{
		MC_DEBUG("FATAL: Too much data for packet, discarding");
		return;
	}

	memcpy(myRawDataBuffer + writeOffset, someData, someDatalen);
	myWriteOffset += static_cast<u16>(someDatalen);
}

void MN_Packet::Clear()
{
	myWriteOffset = 0;
}

MN_Packet::MN_Packet(u8* anAllocatedBuffPtr, u32 theTransportSize)
	: myRawDataBufferLen(theTransportSize)
	, myRawDataBuffer(anAllocatedBuffPtr)
	, myTransportSize(static_cast<u16>(theTransportSize))
	, myWriteOffset(*reinterpret_cast<u16 *>(anAllocatedBuffPtr))
{
	MC_ASSERT(anAllocatedBuffPtr);
	MC_ASSERT(theTransportSize <= std::numeric_limits<u16>::max());

	Clear();
}

MN_Packet::~MN_Packet()
{
	delete[] myRawDataBuffer;
}

void MN_Packet::operator=(const MN_Packet& aPacket)
{
	MC_ASSERT(myRawDataBuffer);
	MC_ASSERT(myRawDataBufferLen >= aPacket.myRawDataBufferLen);

	memcpy(myRawDataBuffer, aPacket.GetBinaryData(), aPacket.GetBinarySize()); // implicit copy of dataSize
	myTransportSize = aPacket.myTransportSize;
}

MN_Packet* MN_Packet::GetNewPacket(u32 theTransportSize)
{
#if 0
	std::scoped_lock locker(GetLocPacketAllocatorMutex());

	MN_Packet* newPacket = nullptr;

	if (!locPacketArray.IsInited())
		locPacketArray.Init(128, 128, false);

	for (int i = 0; i < locPacketArray.size(); i++)
	{
		if (locPacketArray[i]->myTransportSize == theTransportSize)
		{
			newPacket = locPacketArray[i];
			locPacketArray.RemoveCyclicAtIndex(i);
			return newPacket;
		}
	}

	newPacket = new MN_Packet(new unsigned char[theTransportSize], theTransportSize);
	return newPacket;
#endif

#if WIC_MP
	const static u32 dwCall = 0x009D6690;

	__asm
	{
		mov edi, [theTransportSize]
		call [dwCall]
	}
#elif WIC_DS
	return ((MN_Packet *(__cdecl *)(u32))0x00439520)(theTransportSize);
#else
#error Unimplemented
#endif
}
