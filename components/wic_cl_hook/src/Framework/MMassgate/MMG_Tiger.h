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

#include "MMG_ICryptoHashAlgorithm.h"
#include "MMG_CryptoHash.h"

class MMG_Tiger : public MMG_ICryptoHashAlgorithm
{
private:
	u64 myState[3];
	u8 myDataLeftovers[64];
	u32 myDataLeftoversLength;
	u64 myTotalDataLength;

public:
	virtual MMG_CryptoHash GenerateHash(const void* theData, u32 theDataLength) const override;
	virtual HashAlgorithmIdentifier GetIdentifier() const override;

	void Start();
	void Continue(const char* theData, u32 theDataLength);
	MMG_CryptoHash End();
};
static_assert_size(MMG_Tiger, 0x70);