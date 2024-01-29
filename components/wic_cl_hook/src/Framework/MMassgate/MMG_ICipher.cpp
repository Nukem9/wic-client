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
#include "MMG_ICipher.h"

void
MMG_ICipher::SetKey(const char* thePassphrase)
{
	MC_ASSERTMSG(false, "Wrong class instance. You don't belong here.");
}

MMG_ICipher&
MMG_ICipher::operator=(const MMG_ICipher& aRhs)
{
	MC_ASSERTMSG(false, "Wrong class instance. You don't belong here.");
	return *this;
}

void
MMG_ICipher::SetRawKey(const MMG_CryptoHash& theRawKey)
{
	MC_ASSERTMSG(false, "Wrong class instance. You don't belong here.");
}

MMG_CryptoHash
MMG_ICipher::GetHashOfKey() const
{
	MC_ASSERTMSG(false, "Wrong class instance. You don't belong here.");
	return MMG_CryptoHash();
}

void
MMG_ICipher::Encrypt(char* theData, u32 theDataLength) const
{
	MC_ASSERTMSG(false, "Wrong class instance. You don't belong here.");
}

void
MMG_ICipher::Decrypt(char* theData, u32 theDataLength) const
{
	MC_ASSERTMSG(false, "Wrong class instance. You don't belong here.");
}

CipherIdentifier MMG_ICipher::GetIdentifier() const
{
	return CIPHER_UNKNOWN;
}
