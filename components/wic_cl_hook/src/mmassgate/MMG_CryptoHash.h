#pragma once

enum HashAlgorithmIdentifier : uint8_t
{
	HASH_ALGORITHM_UNKNOWN,
	HASH_ALGORITHM_TIGER,
	NUM_HASH_ALGORITHM,
};

class MMG_CryptoHash
{
private:
	uint32_t				m_Hash[16];						// this+0x0
	uint32_t				m_HashLength;					// this+0x40
	HashAlgorithmIdentifier	m_GeneratedFromHashAlgorithm;	// this+0x44

public:
	MMG_CryptoHash();
	MMG_CryptoHash(void *theHash, uint32_t theByteLength, HashAlgorithmIdentifier theSourceAlgorithm);
	~MMG_CryptoHash();

	void SetHash(void *theHash, uint32_t theByteLength, HashAlgorithmIdentifier theSourceAlgorithm);
	uint32_t Get32BitSubset();
};

CHECK_SIZE(MMG_CryptoHash, 0x48);