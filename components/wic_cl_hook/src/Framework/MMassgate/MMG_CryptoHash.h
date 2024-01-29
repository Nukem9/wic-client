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

#include "MMG_ICryptoHashAlgorithm.h"

class MN_WriteMessage;
class MN_ReadMessage;

#define MAX_MMG_CRYPTOHASH_LEN_IN_BYTES_FOR_ALL_ETERNITY 64

class MMG_CryptoHash
{
protected:
	u32 myHash[MAX_MMG_CRYPTOHASH_LEN_IN_BYTES_FOR_ALL_ETERNITY / sizeof(u32)];
	u32 myHashLength;
	HashAlgorithmIdentifier myGeneratedFromHashAlgorithm;

public:
	MMG_CryptoHash();
	MMG_CryptoHash(const MMG_CryptoHash& aRHS);
	MMG_CryptoHash(const void* theHash, u32 theByteLength, HashAlgorithmIdentifier theSourceAlgorithm = HASH_ALGORITHM_UNKNOWN);
	MMG_CryptoHash& operator=(const MMG_CryptoHash& aRHS);
	~MMG_CryptoHash();

	bool operator<(const MMG_CryptoHash& aRhs) const;
	bool operator>(const MMG_CryptoHash& aRhs) const;
	bool operator==(const MMG_CryptoHash& aRHS) const;
	bool operator!=(const MMG_CryptoHash& aRHS) const;

	void SetHash(const void* theHash, u32 theByteLength, HashAlgorithmIdentifier theSourceAlgorithm);
	void GetCopyOfHash(void* theHash, u32 theMaxByteLength) const;
	void Clear();

	// Generic interface to hash subsets, if needed. Please prefer the above interface instead.
	u32 Get32BitSubset() const;
	u64 Get64BitSubset() const;
	HashAlgorithmIdentifier GetGeneratorHashAlgorithmIdentifier() const;

	void ToStream(MN_WriteMessage& theWm) const;
	bool FromStream(MN_ReadMessage& theRm);
};
static_assert_size(MMG_CryptoHash, 0x48);