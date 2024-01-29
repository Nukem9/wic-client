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
#include "MC_HashMap.h"

static u32 SuperFastHash(const char *data, s32 len, u32 hash);

s32 MC_HashMap_SingleHash(u32 aSeed, char aChar)
{
	return (aSeed ^ static_cast<u32>(aChar)) * 214013 + 2531011;
}

void MC_HashMap_HashData(const void* aData, s32 aByteCount, s32 aHashSize, u32* aDest, u32* aSeed)
{
	auto p = static_cast<const char*>(aData);

	if (aSeed)
	{
		for (s32 i = 0; i < aHashSize; i++)
			aDest[i] = aSeed[i];
	}
	else
	{
		for (s32 i = 0; i < aHashSize; i++)
			aDest[i] = 0;
	}

	for (s32 iHash = 0; aByteCount != 0; p++, aByteCount--, iHash = (iHash + 1) % aHashSize)
		aDest[iHash] = MC_HashMap_SingleHash(aDest[iHash], *p);
}

u32 MC_HashMap_HashData(const void* aData, s32 aByteCount, u32 aSeed)
{
	return SuperFastHash(static_cast<const char *>(aData), aByteCount, aSeed);
}

u32 MC_HashMap_HashString(const void *aData, u32 aSeed)
{
	auto p = static_cast<const char *>(aData);
	return SuperFastHash(p, strlen(p), aSeed);
}

static u32 SuperFastHash(const char *data, s32 len, u32 hash)
{
	if (len <= 0 || data == 0)
		return 0;

	auto get16bits = [](const char *data)
	{
		u16 dest = 0;
		memcpy(&dest, data, sizeof(dest));
		return dest;
	};

	const auto rem = len & 3;
	len >>= 2;

	// Main loop
	for (; len > 0; len--)
	{
		hash += get16bits(data);
		const auto tmp = (get16bits(data + 2) << 11) ^ hash;
		hash = (hash << 16) ^ tmp;
		data += 2 * sizeof(u16);
		hash += hash >> 11;
	}

	// Handle end cases
	switch (rem)
	{
	case 3:
		hash += get16bits(data);
		hash ^= hash << 16;
		hash ^= data[sizeof(u16)] << 18;
		hash += hash >> 11;
		break;
	case 2:
		hash += get16bits(data);
		hash ^= hash << 11;
		hash += hash >> 17;
		break;
	case 1:
		hash += *data;
		hash ^= hash << 10;
		hash += hash >> 1;
	}

	// Force "avalanching" of final 127 bits
	hash ^= hash << 3;
	hash += hash >> 5;
	hash ^= hash << 4;
	hash += hash >> 17;
	hash ^= hash << 25;
	hash += hash >> 6;

	return hash;
}