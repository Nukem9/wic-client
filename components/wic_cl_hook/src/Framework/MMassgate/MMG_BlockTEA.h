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

#include "MMG_ICipher.h"

class MMG_BlockTEA : public MMG_ICipher
{
private:
	MMG_CryptoHash myHashOfMyKey;
	s32 myEncryptionKey[4];
	bool myHasValidKey = false;

public:
	MMG_BlockTEA() = default;
	MMG_BlockTEA(const MMG_BlockTEA& aRHS) = delete;
	virtual MMG_BlockTEA& operator=(const MMG_BlockTEA& aRHS);
	~MMG_BlockTEA();

	// Set the password (null-terminated string) to use as the encryption and decryption password
	void SetKey(const char* thePassphrase) override;
	void SetRawKey(const MMG_CryptoHash& theRawKey) override;

	void GetKey(s32 theKey[4]) const;

	// Get the hash of the key used for encryption
	MMG_CryptoHash GetHashOfKey() const override;
	// Encrypt data inplace. Data is theDataLength BYTES long. Data that is 1 single byte WILL NOT BE ENCRYPTED!
	void Encrypt(char* theData, u32 theDataLength) const override;
	// Decrypt data inplace. Data is theDataLength BYTES long
	void Decrypt(char* theData, u32 theDataLength) const override;

	virtual CipherIdentifier GetIdentifier() const override;

private:
	// Encrypt data inplace. Data is theDataLength LONGS long. Data that is 1 single long WILL NOT BE ENCRYPTED!
	void Encrypt(long* theData, u32 theDataLength) const;
	// Decrypt data inplace. Data is theDataLength LONGS long
	void Decrypt(long* theData, u32 theDataLength) const;

	// Do not expose these! Test new instantiations with MMG_EncryptionTester before making specializations public.
	template<typename T> void myEncrypter(T* theData, T theDataLength) const;
	template<typename T> void myDecrypter(T* theData, T theDataLength) const;
	template<typename T> T myCalculateDelta() const;
};
static_assert_size(MMG_BlockTEA, 0x60);