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
#include <math.h>

#include "MMG_BlockTEA.h"
#include "MMG_Tiger.h"

MMG_BlockTEA::~MMG_BlockTEA()
{
	// Avoid using memset here, as it may be optimized away
	for (auto& value : myEncryptionKey)
		value = 0;

	myHasValidKey = false;
}

MMG_BlockTEA&
MMG_BlockTEA::operator=(const MMG_BlockTEA& aRHS)
{
	memcpy(myEncryptionKey, aRHS.myEncryptionKey, sizeof(myEncryptionKey));
	myHasValidKey = aRHS.myHasValidKey;
	myHashOfMyKey = aRHS.myHashOfMyKey;

	return *this;
}

void
MMG_BlockTEA::SetKey(const char* thePassphrase)
{
	MMG_Tiger hasher;

	// The actual 128bit passkey is constructed from a cryptographic hash of the passphrase.
	// We use Tiger (first 128bits) now, which is fast and secure. There should be no need to replace it,
	// however, the code will work with any ICryptoHashAlgorithm of any bitlength.
	// If you change the hasher - just modify above these lines!
	MC_ASSERT(thePassphrase);

	myHashOfMyKey = hasher.GenerateHash(thePassphrase, static_cast<u32>(strlen(thePassphrase)));
	myHashOfMyKey.GetCopyOfHash(myEncryptionKey, sizeof(myEncryptionKey));

	u64 cpHash[4];
	myHashOfMyKey.GetCopyOfHash(cpHash, sizeof(cpHash));
	myHashOfMyKey = hasher.GenerateHash(cpHash, sizeof(cpHash));
	myHasValidKey = true;
}

void
MMG_BlockTEA::SetRawKey(const MMG_CryptoHash& theRawKey)
{
	theRawKey.GetCopyOfHash(myEncryptionKey, sizeof(myEncryptionKey));
}

void MMG_BlockTEA::GetKey(s32 theKey[4]) const
{
	memcpy(theKey, myEncryptionKey, sizeof(myEncryptionKey));
}

MMG_CryptoHash
MMG_BlockTEA::GetHashOfKey() const
{
	return myHashOfMyKey;
}

void
MMG_BlockTEA::Encrypt(char* theData, u32 theDataLength) const
{
	MC_ASSERT(myHasValidKey);

	// Encrypt the first words
	myEncrypter<u32>(reinterpret_cast<u32 *>(theData), static_cast<u32>(theDataLength / 4) & 0xfffffe);
	// Encrypt the trailing few bytes
	myEncrypter<u8>(reinterpret_cast<u8 *>(theData) + (theDataLength & (~7)), static_cast<u8>(theDataLength) & 7);
}

void
MMG_BlockTEA::Decrypt(char* theData, u32 theDataLength) const
{
	MC_ASSERT(myHasValidKey);

	// Decrypt the first words
	myDecrypter<u32>(reinterpret_cast<u32 *>(theData), static_cast<u32>(theDataLength / 4) & 0xfffffe);
	// Decrypt the trailing bytes
	myDecrypter<u8>(reinterpret_cast<u8 *>(theData) + (theDataLength & (~7)), static_cast<u8>(theDataLength) & 7);
}

CipherIdentifier MMG_BlockTEA::GetIdentifier() const
{
	return CIPHER_BLOCKTEA;
}

void
MMG_BlockTEA::Encrypt(long* theData, u32 theDataLength) const
{
	myEncrypter<unsigned long>((unsigned long*)theData, theDataLength);
}

void
MMG_BlockTEA::Decrypt(long* theData, u32 theDataLength) const
{
	myDecrypter<unsigned long>((unsigned long*)theData, theDataLength);
}

// The actual block-TEA encryption algorithm, from the article:
// Correction to XTEA, D. Wheeler, R. Needham, Technical Report, 1998
//
// Templated to handle any datasizes (dec2004 tested 8 and 32bit) less than sizeof(myEncryptionKey)

template<typename T> T
MMG_BlockTEA::myCalculateDelta() const
{
	//return (T) (((double)::pow(2.0, (int)(sizeof(T)*8-1)))*((double)(sqrt(5.0)-1.0)))
	// Would like, but cannot, calculate due to different sqrt() libs etc
	if constexpr (sizeof(T) == 4)		return (T)0x9E3779B9L;
	else if constexpr (sizeof(T) == 1)	return (T)0x9E;
	else if constexpr (sizeof(T) == 8)	return (T)0x9E3779B97F4A7C15LL;
	else if constexpr (sizeof(T) == 2)	return (T)0x9E37;

	MC_ASSERT(false);
	return 0;
}

template<typename T> void
MMG_BlockTEA::myEncrypter(T* theData, T theDataLength) const
{
	static const T delta = myCalculateDelta<T>();
	static const T keymodulo = 16 / sizeof(T) - 1;
	T z = theData[theDataLength - 1];
	T y = theData[0];
	T sum = 0;
	T e, p, q;

	if (theDataLength <= 1)
		return;
	q = (T)(6 + 52 / theDataLength);
	while (q-- > 0)
	{
		sum += delta;
		e = sum >> 2 & 3;
		for (p = 0; p < theDataLength - 1; p++)
			y = theData[p + 1], z = theData[p] += ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + ((((T*)myEncryptionKey)[(p & keymodulo) ^ e]) ^ z));
		y = theData[0];
		z = theData[theDataLength - 1] += ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + ((((T*)myEncryptionKey)[(p & keymodulo) ^ e]) ^ z));
	}
}

template<typename T> void
MMG_BlockTEA::myDecrypter(T* theData, T theDataLength) const
{
	static const T delta = myCalculateDelta<T>();
	static const T keymodulo = 16 / sizeof(T) - 1;
	T z = theData[theDataLength - 1];
	T y = theData[0];
	T sum = 0;
	T e, p, q;

	if (theDataLength <= 1)
		return;
	q = (T)(6 + 52 / theDataLength);
	sum = q * delta;
	while (sum != 0)
	{
		e = sum >> 2 & 3;
		for (p = theDataLength - 1; p > 0; p--)
			z = theData[p - 1], y = theData[p] -= ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + ((((T*)myEncryptionKey)[(p & keymodulo) ^ e]) ^ z));
		z = theData[theDataLength - 1];
		y = theData[0] -= ((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + ((((T*)myEncryptionKey)[(p & keymodulo) ^ e]) ^ z));
		sum -= delta;
	}
}
