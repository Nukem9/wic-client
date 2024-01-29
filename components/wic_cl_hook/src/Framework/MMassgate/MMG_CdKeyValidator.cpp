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
#include <MCommon2/MF_File.h>
#include <cctype>
#include <limits>
#include "MMG_CdKeyValidator.h"
#include "MMG_BitStream.h"
#include "MMG_Tiger.h"

static const char padding1[] = "   ----[  ASCII-EBCDIC  ]---    "; // just padding so the alphabet doesn't get overwritten. ever.
static const char hiddenAlphabet[] = "ABCDEFGHIJKLMNOPRSTUVWXY23456789";
static const char padding2[] = "   ----[  ~ASCII-EBCDIC ]---    "; // just padding so the alphabet doesn't get overwritten. ever.

const char* MMG_CdKey::Validator::LocCdKeyAlphabet = hiddenAlphabet;

bool GetValueOfCharacter(u8 aChar, u8& theValue)
{
	static_assert(sizeof(hiddenAlphabet) == 32 + 1/* zero byte */);

	for (u8 i = 0; i < sizeof(hiddenAlphabet); i++)
	{
		if (MMG_CdKey::Validator::LocCdKeyAlphabet[i] == aChar)
		{
			theValue = i;
			return true;
		}
	}

	return false;
}

MMG_CdKey::Validator::Validator(const MMG_CdKey::Validator& aRhs)
	: myIsChecksumCorrect(aRhs.myIsChecksumCorrect)
{
	memcpy(myKey.data, aRhs.myKey.data, sizeof(myKey));
}

MMG_CdKey::Validator&
MMG_CdKey::Validator::operator=(const MMG_CdKey::Validator& aRhs)
{
	if (this == &aRhs)
		return *this;

	myIsChecksumCorrect = aRhs.myIsChecksumCorrect;
	memcpy(myKey.data, aRhs.myKey.data, sizeof(myKey));
	return *this;
}

void
MMG_CdKey::Validator::SetKey(const char* aKey)
{
#if !WIC_NO_MASSGATE
	if (!aKey)
		return;

	// "sanitize" the key
	u8 key[26];
	u32 keyIndex = 0;
	u8 teckenValue;

	for (s32 i = 0; i < sizeof(key); i++)
	{
		if (aKey[i] == 0)
			break;

		if (aKey[i] != '-')
		{
			key[keyIndex] = aKey[i];

			if (islower(key[keyIndex]))
				key[keyIndex] = static_cast<u8>(toupper(key[keyIndex]));
			if (key[keyIndex] == '1')
				key[keyIndex] = 'I';
			else if (key[keyIndex] == '0')
				key[keyIndex] = 'O';

			if (!GetValueOfCharacter(key[keyIndex], teckenValue))
			{
				MC_ASSERTMSG(false, "Key had an invalid character! Dropping it.");
				return;
			}

			keyIndex++;

			if (keyIndex >= sizeof(key))
			{
				MC_ASSERTMSG(false, "Key too long! Dropping it.");
				return;
			}
		}
	}

	// MODIFIED: Short key lengths are dropped. The game tries to use developer keys in certain
	// parts of the code, which later crashes.
	if (keyIndex < 20)
		return;

	key[keyIndex] = 0;
	memset(myKey.data, 0, sizeof(myKey));

	// Parse chars into bits
	MMG_BitWriter<u8> bw(myKey.data, 128);
	for (u32 i = 0; i < keyIndex; i++)
	{
		MC_ASSERT(key[i] != 0);

		GetValueOfCharacter(key[i], teckenValue);
		bw.WriteBits(teckenValue, 5);
	}

	// Verify checksum
	u8 lastValue = static_cast<u8>((myKey.section.randomDataPart3 << 4) | myKey.section.randomDataPart3);
	u8 descrambledKey[16] = {};
	memcpy(descrambledKey, myKey.data, sizeof(myKey));

	// xor mid with last
	for (s32 i = 2; i < 11; i++)
		descrambledKey[i] ^= lastValue;

	memcpy(myKey.data, descrambledKey, sizeof(myKey));

	// MODIFIED: Now check which product type we want. If we have 'assault.dat' in the root folder, the key
	// type should be upgraded to match. Otherwise downgrade it to the normal edition.
	u32 dateTime = 0;
	u32 size = 0;
	bool hasSA = MF_File::GetFileInfo("assault.dat", dateTime, size, nullptr);

	if (hasSA)
		myKey.section.productId = 3;// Retail WIC:SA key
	else
		myKey.section.productId = 2;// Retail WIC key

	// Compute checksum and compare it with the one embedded in the key
	const auto originalChecksum = myKey.section.checksum;
	myKey.section.checksum = 0x5244;

	MMG_Tiger hasher;
	myKey.section.checksum = hasher.GenerateHash(&myKey, sizeof(myKey)).Get32BitSubset() & 0x3ff;

	// MODIFIED: Key validation always passes
	//if (myKey.section.checksum != originalChecksum)
	//	return;

	myIsChecksumCorrect = true;
#endif
}

void
MMG_CdKey::Validator::ScrambleKey(u8* aKey) const
{
	MC_ASSERTMSG(aKey && aKey[0], "Null or empty string is not supported.");

	u32 index = 1;
	const u8 exxoor = aKey[0];

	while (aKey[index])
		aKey[index++] ^= exxoor;
}

bool
MMG_CdKey::Validator::IsKeyValid() const
{
	return myIsChecksumCorrect;
}

bool
MMG_CdKey::Validator::GetEncryptionKey(EncryptionKey& theKey) const
{
	MC_ASSERT(myIsChecksumCorrect);

	if (!myIsChecksumCorrect)
		return false;

	MMG_BitReader<u8> encryptionString(myKey.data, 128);
	encryptionString.ReadBits(5); // Read past sequenceNumber
	encryptionString.ReadBits(5); // Read past sequenceNumber
	encryptionString.ReadBits(5); // Read past sequenceNumber
	encryptionString.ReadBits(5); // Read past sequenceNumber
	encryptionString.ReadBits(5); // Read past sequenceNumber
	encryptionString.ReadBits(5); // Read past sequenceNumber
	encryptionString.ReadBits(5); // Read past sequenceNumber

	int buffIndex = 0;
	char encryptionKeyAsString[32];

	while (buffIndex < 13)
	{
		MC_ASSERT(encryptionString.GetStatus() != MMG_BitStream::EndOfStream);

		encryptionKeyAsString[buffIndex] = MMG_CdKey::Validator::LocCdKeyAlphabet[encryptionString.ReadBits(5)];
		buffIndex++;

		MC_ASSERT(buffIndex < sizeof(encryptionKeyAsString));
	}

	MC_ASSERT(buffIndex);

	encryptionKeyAsString[buffIndex] = 0;
	theKey = encryptionKeyAsString;
	return true;
}

u32
MMG_CdKey::Validator::GetSequenceNumber() const
{
	MC_ASSERT(myIsChecksumCorrect);

	if (!myIsChecksumCorrect)
		return std::numeric_limits<u32>::max();

	return myKey.section.sequenceNumber;
}

u32
MMG_CdKey::Validator::GetBatchId() const
{
	MC_ASSERT(myIsChecksumCorrect);

	if (!myIsChecksumCorrect)
		return std::numeric_limits<u32>::max();

	return myKey.section.batchId;
}

u8
MMG_CdKey::Validator::GetProductIdentifier() const
{
	MC_ASSERT(myIsChecksumCorrect);

	if (!myIsChecksumCorrect)
		return std::numeric_limits<u8>::max();

	return myKey.section.productId;
}

void
MMG_AccessCode::Validator::SetCode(const char* aCode)
{
#if !WIC_NO_MASSGATE
	if (!aCode)
		return;

	// "sanitize" the code
	u8 code[16];
	u32 codeIndex = 0;
	u8 teckenValue;

	for (s32 i = 0; i < sizeof(code); i++)
	{
		if (aCode[i] == 0)
			break;

		if (aCode[i] != '-')
		{
			code[codeIndex] = aCode[i];

			if (islower(code[codeIndex]))
				code[codeIndex] = static_cast<u8>(toupper(code[codeIndex]));
			if (code[codeIndex] == '1')
				code[codeIndex] = 'I';
			else if (code[codeIndex] == '0')
				code[codeIndex] = 'O';

			if (!GetValueOfCharacter(code[codeIndex], teckenValue))
				return;

			codeIndex++;
			MC_ASSERT(codeIndex < sizeof(code));
		}
	}

	code[codeIndex] = 0;
	memset(myCode.data, 0, sizeof(myCode));
	
	// Parse chars into bits
	MMG_BitWriter<u8> bw(myCode.data, 64);
	for (u32 i = 0; i < codeIndex; i++)
	{
		MC_ASSERT(code[i] != 0);

		GetValueOfCharacter(code[i], teckenValue);
		bw.WriteBits(teckenValue, 5);
	}

	u8 lastValue = static_cast<u8>(myCode.data[6]);
	u8 descrambledCode[9] = {};
	memcpy(descrambledCode, myCode.data, sizeof(myCode));

	// xor mid with last
	for (s32 i = 2; i < 6; i++)
		descrambledCode[i] ^= lastValue;

	memcpy(myCode.data, descrambledCode, sizeof(myCode));

	const auto originalChecksum = myCode.section.checksum;
	myCode.section.checksum = 0x5244;

	MMG_Tiger hasher;
	myCode.section.checksum = hasher.GenerateHash(&myCode, sizeof(myCode)).Get32BitSubset() & 0x1f;

	if (myCode.section.checksum != originalChecksum)
		return;

	myIsChecksumCorrect = true;
#endif
}

bool
MMG_AccessCode::Validator::IsCodeValid() const
{
	return myIsChecksumCorrect;
}

u32
MMG_AccessCode::Validator::GetTypeOfCode() const
{
	return myCode.section.codeType;
}

u32
MMG_AccessCode::Validator::GetSequenceNumber() const
{
	return myCode.section.sequenceNumber;
}

u32
MMG_AccessCode::Validator::GetKeyData() const
{
	return myCode.section.secretCode;
}

u32
MMG_AccessCode::Validator::GetProductId() const
{
	return myCode.section.productId;
}