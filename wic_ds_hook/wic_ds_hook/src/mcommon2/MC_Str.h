#pragma once

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