#pragma once

#include "../types.h"

template<typename T, uint Length>
class MC_Str
{
	union
	{
		T		*myDynamicPointer;
		int		*myLength;
		T		myBuffer[Length];
	};
};

template<>
class MC_Str<char, 0>
{
	union
	{
		char	*myDynamicPointer;
		int		*myLength;
		char	myBuffer[1];
	};
};

template<>
class MC_Str<wchar_t, 0>
{
	union
	{
		wchar_t	*myDynamicPointer;
		int		*myLength;
		wchar_t	myBuffer[1];
	};
};

static_assert(sizeof(MC_Str<char, 0>) == 0x4, "Invalid class size");
static_assert(sizeof(MC_Str<wchar_t, 0>) == 0x4, "Invalid class size");

static int MC_StringToInt(const char *const aString)
{
	const char *v1; // edx
	unsigned __int8 v2; // al
	unsigned int v3; // esi
	unsigned int i; // ecx

	v1 = aString;
	v2 = *aString;
	v3 = 0;
	for (i = 1; v2; v3 += i)
	{
		i += v2;
		v2 = (v1++)[1];
	}
	return ((v3 + 15 * (v3 / 0xFFF1)) << 16) | i % 0xFFF1;
}