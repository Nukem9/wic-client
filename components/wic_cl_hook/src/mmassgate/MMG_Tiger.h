#pragma once

class MMG_Tiger
{
public:

private:
	unsigned __int64 myState[3];
	char myDataLeftovers[64];
	unsigned int myDataLeftoversLength;
	unsigned __int64 myTotalDataLength;

public:
	MMG_Tiger();

	MMG_CryptoHash GenerateHash(const void *theData, unsigned int theDataLength);
};