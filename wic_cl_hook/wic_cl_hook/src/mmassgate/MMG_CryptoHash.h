#pragma once

class MMG_CryptoHash 
{
public:
	ulong	myHash[16];						// this+0x0
	ulong	myHashLength;					// this+0x40
	uint	myGeneratedFromHashAlgorithm;	// this+0x44
};

CHECK_SIZE(MMG_CryptoHash, 0x48);