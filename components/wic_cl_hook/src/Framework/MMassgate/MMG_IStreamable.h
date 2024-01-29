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

class MN_ReadMessage;
class MN_WriteMessage;

class MMG_IStreamable
{
public:
	// Write the contents to the stream
	virtual void ToStream(MN_WriteMessage& theStream) const = 0;

	// Read contents from stream. Returns false if stream is corrupt.
	virtual bool FromStream(MN_ReadMessage& theStream) = 0;

	// Default comparison operator. Always returns true as there's nothing to compare.
	bool operator==(const MMG_IStreamable& aOther) const = default;
};