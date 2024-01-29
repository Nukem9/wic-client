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
#include <MNetwork/MN_WriteMessage.h>
#include <MNetwork/MN_ReadMessage.h>
#include "MMG_CryptoHash.h"

MMG_CryptoHash::MMG_CryptoHash()
	: myHashLength(0)
{
	Clear();
	myGeneratedFromHashAlgorithm = HASH_ALGORITHM_UNKNOWN;
}

MMG_CryptoHash::MMG_CryptoHash(const MMG_CryptoHash& aRHS)
{
	SetHash(&aRHS.myHash, aRHS.myHashLength, aRHS.myGeneratedFromHashAlgorithm);
}

MMG_CryptoHash::MMG_CryptoHash(const void* theHash, u32 theByteLength, HashAlgorithmIdentifier theSourceAlgorithm)
{
	SetHash(theHash, theByteLength, theSourceAlgorithm);
}

MMG_CryptoHash::~MMG_CryptoHash()
{
	Clear();
}

MMG_CryptoHash&
MMG_CryptoHash::operator=(const MMG_CryptoHash& aRHS)
{
	if (this == &aRHS)
		return *this;

	SetHash(&aRHS.myHash, aRHS.myHashLength, aRHS.myGeneratedFromHashAlgorithm);
	return *this;
}

bool
MMG_CryptoHash::operator<(const MMG_CryptoHash& aRhs) const
{
	MC_ASSERT(this->myGeneratedFromHashAlgorithm == aRhs.myGeneratedFromHashAlgorithm);
	return memcmp(myHash, aRhs.myHash, myHashLength) < 0;
}

bool
MMG_CryptoHash::operator>(const MMG_CryptoHash& aRhs) const
{
	MC_ASSERT(this->myGeneratedFromHashAlgorithm == aRhs.myGeneratedFromHashAlgorithm);
	return memcmp(myHash, aRhs.myHash, myHashLength) > 0;
}

bool
MMG_CryptoHash::operator==(const MMG_CryptoHash& aRHS) const
{
	return
		myGeneratedFromHashAlgorithm == aRHS.myGeneratedFromHashAlgorithm &&
		myHashLength == aRHS.myHashLength &&
		memcmp(myHash, aRHS.myHash, myHashLength) == 0;
}

bool
MMG_CryptoHash::operator!=(const MMG_CryptoHash& aRHS) const
{
	return !(*this == aRHS);
}

void
MMG_CryptoHash::SetHash(const void* theHash, u32 theByteLength, HashAlgorithmIdentifier theSourceAlgorithm)
{
	MC_ASSERT(theByteLength < sizeof(myHash));

	Clear();
	myHashLength = theByteLength;
	myGeneratedFromHashAlgorithm = theSourceAlgorithm;
	memcpy(myHash, theHash, theByteLength);
}

void
MMG_CryptoHash::GetCopyOfHash(void* theHash, u32 theMaxByteLength) const
{
	memset(theHash, 0, theMaxByteLength);
	memcpy(theHash, myHash, std::min(myHashLength, theMaxByteLength));
}

void
MMG_CryptoHash::Clear()
{
	memset(myHash, 0, sizeof(myHash));
	myHashLength = 0;
	myGeneratedFromHashAlgorithm = HASH_ALGORITHM_UNKNOWN;
}

u32
MMG_CryptoHash::Get32BitSubset() const
{
	MC_ASSERT(myHashLength >= sizeof(u32));

	u32 subset = 0;
	memcpy(&subset, myHash, sizeof(subset));

	return subset;
}

u64
MMG_CryptoHash::Get64BitSubset() const
{
	MC_ASSERT(myHashLength >= sizeof(u64));

	u64 subset = 0;
	memcpy(&subset, myHash, sizeof(subset));

	return subset;
}

HashAlgorithmIdentifier
MMG_CryptoHash::GetGeneratorHashAlgorithmIdentifier() const
{
	return myGeneratedFromHashAlgorithm;
}

void
MMG_CryptoHash::ToStream(MN_WriteMessage& theWm) const
{
	theWm.WriteUChar(static_cast<u8>(myHashLength));
	theWm.WriteUChar(static_cast<u8>(myGeneratedFromHashAlgorithm));
	theWm.WriteRawData(myHash, static_cast<u16>(myHashLength));
}

bool
MMG_CryptoHash::FromStream(MN_ReadMessage& theRm)
{
	u8 hashlen = 0;
	u8 generatedfrom = 0;

	if (!theRm.ReadUChar(hashlen) || hashlen > sizeof(myHash))
		return false;

	if (!theRm.ReadUChar(generatedfrom))
		return false;

	if (!theRm.ReadRawData(&myHash, hashlen))
		return false;

	myHashLength = hashlen;
	myGeneratedFromHashAlgorithm = static_cast<HashAlgorithmIdentifier>(generatedfrom);
	return true;
}