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

template<class Type>
class MC_Array
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

protected:
	Type* myNodes = nullptr;
	u32 myNumNodes = 0;
	u32 myNextFreeNode = 0;

public:
	MC_Array() = delete;

	MC_Array(u32 aNumNodes)
	{
		myNumNodes = aNumNodes;
		myNodes = new Type[myNumNodes];

		MC_ASSERT(myNodes);
	}

	~MC_Array()
	{
		clear();

		if (myNodes)
		{
			delete[] myNodes;
			myNodes = nullptr;
		}
	}

	MC_Array(const MC_Array& anOther) = delete;
	MC_Array<Type>& operator=(const MC_Array& anOther) = delete;

	Type& operator[](size_t anIndex)
	{
#if MC_HEAVY_DEBUG_GROWINGARRAY_BOUNDSCHECK
		MC_ASSERT(anIndex < size());
#endif

		return myNodes[anIndex];
	}

	const Type& operator[](size_t anIndex) const
	{
#if MC_HEAVY_DEBUG_GROWINGARRAY_BOUNDSCHECK
		MC_ASSERT(anIndex < size());
#endif

		return myNodes[anIndex];
	}

	Type *data()
	{
		return myNodes;
	}

	const Type *data() const
	{
		return myNodes;
	}

	Type *begin()
	{
		return myNodes;
	}

	const Type *begin() const
	{
		return myNodes;
	}

	Type *end()
	{
		return myNodes + size();
	}

	const Type *end() const
	{
		return myNodes + size();
	}

	bool empty() const
	{
		return size() == 0;
	}

	size_t size() const
	{
		return myNextFreeNode;
	}

	void clear()
	{
		if (size())
			myNextFreeNode = 0;
	}
};