#include "../stdafx.h"

DPTR(MN_Message, ourZipFlag);
DPTR(MN_Message, ourCompressionFlag);
DPTR(MN_Message, ourDefaultTypeCheckFlag);

/*
MN_Message::MN_Message()
{
	this->ourTypeCheckFlag = ourDefaultTypeCheckFlag;
	this->myPendingPackets.myMaxSize = 8;
	this->myPendingPackets.myCount = 0;
	this->myPendingPackets.myData = this->myPendingPackets.myStaticData;
	this->myCurrentPacket = 0;
	this->myLastDelimiter = 0;
	this->myLastDelimiterWriteOffset = 0;
	this->myPendingPacketMessageSize = 0;
}
*/

bool MN_Message::Empty()
{
	return this->myCurrentPacket == nullptr;
}

DPTR(MN_MessageOnlineExeDisc, ourZipFlag);
DPTR(MN_MessageOnlineExeDisc, ourCompressionFlag);
DPTR(MN_MessageOnlineExeDisc, ourDefaultTypeCheckFlag);

bool MN_MessageOnlineExeDisc::Empty()
{
	return this->myCurrentPacket == nullptr;
}