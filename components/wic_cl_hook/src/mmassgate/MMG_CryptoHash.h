#pragma once

enum HashAlgorithmIdentifier : uchar
{
	HASH_ALGORITHM_UNKNOWN,
	HASH_ALGORITHM_TIGER,
	NUM_HASH_ALGORITHM,
};

class MMG_CryptoHash
{
private:
	ulong					m_Hash[16];						// this+0x0
	ulong					m_HashLength;					// this+0x40
	HashAlgorithmIdentifier	m_GeneratedFromHashAlgorithm;	// this+0x44

public:
	MMG_CryptoHash();
	MMG_CryptoHash(voidptr_t theHash, uint theByteLength, HashAlgorithmIdentifier theSourceAlgorithm);
	~MMG_CryptoHash();

	void SetHash(voidptr_t theHash, uint theByteLength, HashAlgorithmIdentifier theSourceAlgorithm);
	uint Get32BitSubset();
};

CHECK_SIZE(MMG_CryptoHash, 0x48);