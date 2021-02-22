#include "../stdafx.h"

const char *MMG_CdKey::Validator::LocCdKeyAlphabet = "ABCDEFGHIJKLMNOPRSTUVWXY23456789";

void MMG_CdKey::Validator::SetKey(const char *aKey)
{
	if (!aKey)
		return;

	char key[26];
	int keyIndex = 0;
	int theValue = 0;

	// Determine the key string length and convert AAAA-BBBB-CCCC-DDDD-EEEE to AAAABBBBCCCCDDDDEEEE
	for (int i = 0; i < ARRAYSIZE(key); i++)
	{
		if (aKey[i] == 0)
			break;

		if (aKey[i] == '-')
			continue;

		// Write single char to buffer and validate alphanumeric index (less than 33)
		key[keyIndex] = aKey[i];

		if (islower(aKey[i]))
			key[keyIndex] -= ' ';

		if (key[keyIndex] == '1')
			key[keyIndex] = 'I';
		else if (key[keyIndex] == '0')
			key[keyIndex] = 'O';

		int v7 = 0;
		while (LocCdKeyAlphabet[v7] != key[keyIndex])
		{
			++v7;

			if (v7 >= 33)
				return;
		}

		keyIndex++;
		theValue = v7;

		MC_ASSERT(keyIndex < ARRAYSIZE(key));
	}

	memset(this->m_Key.data, 0, sizeof(this->m_Key.data));

	int i = 0;
	key[keyIndex] = 0;

	MMG_BitWriter<unsigned char> bw((unsigned char *)&this->m_Key.data, 128);

	if (keyIndex)
	{
		do
		{
			MC_ASSERT(key[i] != 0);

			int v10 = 0;
			while (LocCdKeyAlphabet[v10] != key[i])
			{
				++v10;
				if (v10 >= 33)
					goto LABEL_30;
			}
			theValue = v10;
		LABEL_30:
			bw.WriteBits(theValue, 5);
			i++;
		} while (i < keyIndex);
	}

	KeyDefinition descrambledKey;
	descrambledKey.section = this->m_Key.section;

	uchar tekenValue = (uchar)(this->m_Key.section.randomDataPart3 << 4 | this->m_Key.section.randomDataPart3);

	for (int xorIndex = 2; xorIndex < 11; xorIndex++)
		descrambledKey.data[xorIndex] ^= tekenValue;

	this->m_Key.section = descrambledKey.section;

	unsigned int v17 = *(DWORD *)&this->m_Key.data[0];
	unsigned int v18 = *(DWORD *)&this->m_Key.data[4] >> 3;

	*(DWORD *)&this->m_Key.data[4] = *(DWORD *)(&this->m_Key.data[4]) & 0xFFFFF227 | 0x1220;
	*(DWORD *)&this->m_Key.data[0] = v17;

	int v19 = v18 & 0x3FF;

	MMG_Tiger mmgTiger;
	auto cryptoHash = mmgTiger.GenerateHash(this->m_Key.data, sizeof(this->m_Key.data));

	this->m_Key.section.checksum = cryptoHash.Get32BitSubset() & 0x3FF;

	if (this->m_Key.section.checksum == v19)
	{
		this->m_IsChecksumCorrect = true;

		// 'assault.dat' indicates SA files are present
		uint dateTime;
		uint size;
		bool hasSA = MF_File::GetFileInfo("assault.dat", dateTime, size, nullptr);

		// Checksum is good, now check which key type we want. If we have 'assault.dat' in
		// the root folder, key type should always be upgraded to match. Otherwise downgrade it.
		if (hasSA)
			this->m_Key.section.productId = 3;// Retail WIC:SA key
		else
			this->m_Key.section.productId = 2;// Retail WIC key
	}

}

uint MMG_CdKey::Validator::GetSequenceNumber()
{
	MC_ASSERT(this->m_IsChecksumCorrect);

	if (this->m_IsChecksumCorrect)
		return this->m_Key.section.sequenceNumber;

	return -1;
}

void __declspec(naked) MMG_CdKey::Validator::hk_SetKey()
{
	__asm
	{
		push dword ptr[esp + 4h]
		mov ecx, esi
		call offset MMG_CdKey::Validator::SetKey
		retn 4
	}
}

void MMG_CdKey::InitializeHook()
{
	Detours::X86::DetourFunctionClass(reinterpret_cast<uint8_t *>(0x0079C3A0), &MMG_CdKey::Validator::hk_SetKey);
}