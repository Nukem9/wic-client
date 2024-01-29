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
#include "MN_Packet.h"
#include "MN_Message.h"

void MN_Message::EnableZipCompression(bool aState)
{
	ourZipFlag = aState;
}

void MN_Message::EnableCompression(bool aState)
{
	ourCompressionFlag = aState;
}

void MN_Message::EnableTypeChecking(bool aState)
{
	ourDefaultTypeCheckFlag = aState;
}

bool MN_Message::ShouldZip()
{
	return ourZipFlag;
}

bool MN_Message::IsCompressed()
{
	return ourCompressionFlag;
}

bool MN_Message::IsTypeChecked()
{
	return ourDefaultTypeCheckFlag;
}

MN_Message::MN_Message()
	: ourTypeCheckFlag(ourDefaultTypeCheckFlag)
{
	// Nothing allocated from beginning
}

MN_Message::~MN_Message()
{
	Clear();
}

void MN_Message::Clear()
{
	// Delete ALL packets
	if (myCurrentPacket)
		MN_Packet::Deallocate(myCurrentPacket);

	for (auto& packet : myPendingPackets)
		MN_Packet::Deallocate(packet);

	myPendingPackets.clear();

	myLastDelimiterWriteOffset = 0;
	myPendingPacketMessageSize = 0;
}

bool MN_Message::Empty()
{
	return myCurrentPacket == nullptr;
}

u32 MN_Message::GetMessageSize()
{
	if (myCurrentPacket)
		return myPendingPacketMessageSize + myCurrentPacket->GetBinarySize();

	return myPendingPacketMessageSize;
}

MN_DelimiterType MN_Message::GetLastDelimiter()
{
	return myLastDelimiter;
}
