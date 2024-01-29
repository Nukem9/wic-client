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

// Abstract base classes for formatted data types to be sent via mn_connections

#include "MN_ConnectionErrorType.h"

class MN_IWriteableDataStream;

// Sending
class MN_SendFormat
{
protected:
	MN_SendFormat() = default;

public:
	virtual ~MN_SendFormat() = default;
	virtual MN_ConnectionErrorType SendMe(MN_IWriteableDataStream* aConnection, bool aDisableCompression = false) = 0;
};
static_assert_size(MN_SendFormat, 0x4);

// Receiving
class MN_ReceiveFormat
{
protected:
	MN_ReceiveFormat() = default;

public:
	virtual ~MN_ReceiveFormat() = default;

	// Should return the number of bytes used by the subclass implementation, 0 if none used.
	virtual u32 BuildMe(const void* aBuffer, const u32 aBufferLength) = 0;
	virtual bool AtEnd() = 0;
};
static_assert_size(MN_ReceiveFormat, 0x4);
