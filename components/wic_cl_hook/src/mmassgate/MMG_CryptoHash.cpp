#include "../stdafx.h"

MMG_CryptoHash::MMG_CryptoHash()
{
	memset(this->m_Hash, 0, sizeof(this->m_Hash));
	this->m_HashLength = 0;
	this->m_GeneratedFromHashAlgorithm = HASH_ALGORITHM_UNKNOWN;
}

MMG_CryptoHash::MMG_CryptoHash(void *theHash, uint32_t theByteLength, HashAlgorithmIdentifier theSourceAlgorithm)
{
	this->SetHash(theHash, theByteLength, theSourceAlgorithm);
}

MMG_CryptoHash::~MMG_CryptoHash()
{
	memset(this->m_Hash, 0, sizeof(this->m_Hash));
	this->m_HashLength = 0;
	this->m_GeneratedFromHashAlgorithm = HASH_ALGORITHM_UNKNOWN;
}

void MMG_CryptoHash::SetHash(void *theHash, uint32_t theByteLength, HashAlgorithmIdentifier theSourceAlgorithm)
{
	MC_ASSERT(theByteLength < sizeof(m_Hash));

	this->m_HashLength = theByteLength;
	this->m_GeneratedFromHashAlgorithm = theSourceAlgorithm;

	// Zero all bits and then copy over the incoming data
	memset(this->m_Hash, 0, sizeof(this->m_Hash));
	memcpy(this->m_Hash, theHash, theByteLength);
}

uint32_t MMG_CryptoHash::Get32BitSubset()
{
	MC_ASSERT(m_HashLength >= sizeof(uint32_t));

	return this->m_Hash[0];
}