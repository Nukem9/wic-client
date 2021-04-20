#pragma once

template<uint32_t Length>
class MC_StaticString : MC_Str<char, Length>
{
public:
};

static_assert(sizeof(MC_StaticString<0>) == 0x4, "Invalid class size");