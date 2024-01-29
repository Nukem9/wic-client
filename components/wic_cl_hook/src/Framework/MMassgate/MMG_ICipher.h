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

#include "MMG_CryptoHash.h"

enum CipherIdentifier
{
	CIPHER_UNKNOWN = 0,
	CIPHER_BLOCKTEA,
	CIPHER_NULLCIPHER,
	CIPHER_ILLEGAL_CIPHER,
	NUM_CIPHERS,
};

// NOTE: This class is not abstract since we want to be able to pass arbitrary ICiphers on the stack
class MMG_ICipher
{
public:
	virtual MMG_ICipher& operator=(const MMG_ICipher& aRHS);
	virtual void SetKey(const char* thePassphrase);
	virtual void SetRawKey(const MMG_CryptoHash& theRawKey);
	virtual MMG_CryptoHash GetHashOfKey() const;
	virtual void Encrypt(char* theData, u32 theDataLength) const;
	virtual void Decrypt(char* theData, u32 theDataLength) const;
	virtual CipherIdentifier GetIdentifier() const;
};
static_assert_size(MMG_ICipher, 0x4);
