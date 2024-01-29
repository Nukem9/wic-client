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

#include <MCommon2/MC_String.h>

class MMG_CdKey
{
public:
	#pragma pack(push, 1)
	union KeyDefinition
	{
		struct
		{	// 100 bit key, with 55bit random data (for a five groups of four characters style key)
			u64 productId : 3;			//
			u64 batchId : 7;			// 10
			u64 sequenceNumber : 25;	// 35
			u64 checksum : 10;			// 50
			u64 randomDataPart1 : 19;	// 64
			u64 randomDataPart2 : 32;	// 96
			u64 randomDataPart3 : 4;	// 100
			u64 zero : 28;				// 128
		} section;

		u8 data[16];
	};
	#pragma pack(pop)

	class Validator
	{
	public:
		static const char* LocCdKeyAlphabet;

		const static u32 MAX_CDKEY_LENGTH = 32;
		const static u32 MAX_ENCRYPTIONKEYSTRING_LENGTH = 14;

		using EncryptionKey = MC_StaticString<MAX_ENCRYPTIONKEYSTRING_LENGTH>;

		enum KeyType
		{
			INVALID_TYPE,

			PERMANENT_TEA_ENCRYPTION,
			PERMANENT_AES_ENCRYPTION,
			PERMANENT_BLOWFISH_ENCRYPTION,

			SUBSCRIPTION_TEA_ENCRYPTION,
			SUBSCRIPTION_AES_ENCRYPTION,
			SUBSCRIPTION_BLOWFISH_ENCRYPTION,

			CAFE_TEA_ENCRYPTION,
			CAFE_AES_ENCRYPTION,
			CAFE_BLOWFISH_ENCRYPTION,

			GUEST_KEY_TEA_ENCRYPTION,
		};

		Validator() = default;
		Validator(const Validator& aRhs);
		~Validator() = default;
		Validator& operator=(const Validator& aRhs);

		void SetKey(const char* aKey);

		// Just scramble the key for storage in e.g. registry. Call again to descramble.
		void ScrambleKey(u8* aKey) const;

		bool IsKeyValid() const;
		bool GetEncryptionKey(EncryptionKey& theKey) const;
		u32 GetSequenceNumber() const;
		u32 GetBatchId() const;
		u8 GetProductIdentifier() const;

	private:
		KeyDefinition myKey = {};
		bool myIsChecksumCorrect = false;
	};
	static_assert_size(Validator, 0x11);
};

class MMG_AccessCode
{
public:
	#pragma pack(push, 1)
	union CodeDefinition
	{
		struct
		{
			// 60 bit code, ASDF-ASDF-ASDF
			u64 productId : 3;
			u64 codeType : 5;
			u64 batchId : 7;
			u64 checksum : 5;
			u64 sequenceNumber : 20;
			u64 secretCode : 20;
		} section;

		u8 data[8];
	};
	#pragma pack(pop)

	class Validator
	{
	public:
		Validator() = default;
		~Validator() = default;

		void SetCode(const char* aCode);
		bool IsCodeValid() const;
		u32 GetProductId() const;
		u32 GetTypeOfCode() const;
		u32 GetSequenceNumber() const;
		u32 GetKeyData() const;

	private:
		CodeDefinition myCode = {};
		bool myIsChecksumCorrect = false;
	};
	static_assert_size(Validator, 0x9);
};