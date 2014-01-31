#pragma once

class MMG_BlockTEA : public MMG_ICipher
{
public:
	MMG_CryptoHash	myHashOfMyKey;		// this+0x4
	long			myEncryptionKey[4];	// this+0x4C
	bool			myHasValidKey;		// this+0x5C
};

CHECK_SIZE(MMG_BlockTEA, 0x60);