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

#include "MC_HeapImplementation.h"

template <typename Type, int Size>
class MC_StaticArray
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

protected:
	Type myItemList[Size];

public:
	MC_StaticArray() = default;

	// Copy constructor not allowed. To avoid duplicating the array by mistake.
	MC_StaticArray(const MC_StaticArray& anArray) = delete;

	Type& operator[](size_t anIndex)
	{
#if MC_HEAVY_DEBUG_GROWINGARRAY_BOUNDSCHECK
		MC_ASSERTMSG(anIndex < size(), "MC_StaticArray BOUNDS ERROR!");
#endif

		return myItemList[anIndex];
	}

	const Type& operator[](size_t anIndex) const
	{
#if MC_HEAVY_DEBUG_GROWINGARRAY_BOUNDSCHECK
		MC_ASSERTMSG(anIndex < size(), "MC_StaticArray BOUNDS ERROR!");
#endif

		return myItemList[anIndex];
	}

	Type *data()
	{
		return myItemList;
	}

	const Type *data() const
	{
		return myItemList;
	}

	Type *begin()
	{
		return myItemList;
	}

	const Type *begin() const
	{
		return myItemList;
	}

	Type *end()
	{
		return myItemList + size();
	}

	const Type *end() const
	{
		return myItemList + size();
	}

	bool empty() const
	{
		return size() == 0;
	}

	static size_t size()
	{
		return Size;
	}

	bool operator==(const MC_StaticArray& anArray) const
	{
		for (int i = 0; i < Size; i++)
		{
			if (!(myItemList[i] == anArray.myItemList[i]))
				return false;
		}

		return true;
	}

	bool operator!=(const MC_StaticArray& anArray) const
	{
		for (int i = 0; i < Size; i++)
		{
			if (myItemList[i] != anArray.myItemList[i])
				return true;
		}

		return false;
	}
};

template<typename Type, int Size, int Size2>
class MC_StaticArray2 : public MC_StaticArray<MC_StaticArray<Type, Size2>, Size>
{
};

template<typename Type, int Size, int Size2, int Size3>
class MC_StaticArray3 : public MC_StaticArray<MC_StaticArray<MC_StaticArray<Type, Size3>, Size2>, Size>
{
};