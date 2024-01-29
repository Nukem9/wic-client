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

// Standard cross platform data types and support functions
// This file gets included by MC_Base.h (which in turn gets included by MC_GlobalDefines.h)

#include <cstdint>
#include <cstring>
#include <algorithm>

typedef std::uint64_t		u64;
typedef std::int64_t		s64;
typedef std::uint32_t		u32;
typedef std::int32_t		s32;
typedef std::uint16_t		u16;
typedef std::int16_t		s16;
typedef std::uint8_t		u8;
typedef std::int8_t			s8;

static_assert(sizeof(s32) == sizeof(long));
static_assert(sizeof(u32) == sizeof(unsigned long));
static_assert(sizeof(s64) == sizeof(long long));
static_assert(sizeof(u64) == sizeof(unsigned long long));

// Alternate long data types
typedef u64					uint64;
typedef s64					int64;
typedef u32					uint32;
typedef s32					int32;
typedef u16					uint16;
typedef s16					int16;
typedef u8					uint8;
typedef s8					int8;

// F32 defines
#define F32_PI				(3.141592653589793f)
#define F32_HALF_PI			(F32_PI*0.5f)
#define F32_TWO_PI			(F32_PI*2.0f)
#define F32_DEGTORAD(ang)	((f32)(ang)*(F32_TWO_PI*(1.0f/360.0f)))
#define	F32_RADTODEG(ang)	((f32)(ang)*(360.0f/F32_TWO_PI))

// F64 defines
#define F64_PI				(3.141592653589793)
#define F64_HALF_PI			(F64_PI*0.5)
#define F64_TWO_PI			(F64_PI*2.0)
#define F64_DEGTORAD(ang)	((f64)(ang)*(F64_TWO_PI*(1.0/360.0)))
#define	F64_RADTODEG(ang)	((f64)(ang)*(360.0/F64_TWO_PI))

enum McNoInitType
{
	MC_NO_INIT
};