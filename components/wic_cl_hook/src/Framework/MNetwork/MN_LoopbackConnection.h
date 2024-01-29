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

#include "MN_IWriteableDataStream.h"

template<s32 BUFFERSIZE = 20480>
class MN_LoopbackConnection : public MN_IWriteableDataStream
{
private:
	u8 myData[BUFFERSIZE];
	u32 myDatalen = 0;

public:
	MN_LoopbackConnection() = default;

	MN_ConnectionErrorType Send(const void *theData, u32 theDatalen) override
	{
		if (myDatalen + theDatalen > BUFFERSIZE)
			return MN_CONN_BROKEN;

		memcpy(myData + myDatalen, theData, theDatalen);
		myDatalen += theDatalen;

		return MN_CONN_OK;
	}

	void Reset()
	{
		myDatalen = 0;
	}

	u32 GetRecommendedBufferSize() const override
	{
		return BUFFERSIZE;
	}

	const u8 *GetData() const
	{
		return myData;
	}

	u32 GetDataLength() const
	{
		return myDatalen;
	}
};