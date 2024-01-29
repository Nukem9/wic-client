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

#include <memory>
#include "MC_HeapImplementation.h"

template<typename Type, int StaticSize>
class MC_HybridArray
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

private:
	s32 myCount = 0;
	s32 myMaxSize = 0;
	Type *myData = nullptr;
	Type myStaticData[StaticSize];

public:
	MC_HybridArray()
	{
		memset(myStaticData, 0xFE, sizeof(myStaticData));

		myData = myStaticData;
		myMaxSize = StaticSize;
	}

	~MC_HybridArray()
	{
		// If not static data, it was allocated with new[] and must be deleted
		if (reinterpret_cast<std::uintptr_t>(myData) != reinterpret_cast<std::uintptr_t>(myStaticData))
			delete[] myData;
	}

	MC_HybridArray(const MC_HybridArray& anOther) = delete;
	MC_HybridArray<Type, StaticSize>& operator=(const MC_HybridArray& anOther) = delete;

	template<int OtherSize>
	MC_HybridArray(const MC_HybridArray<Type, OtherSize>& anOther) = delete;

	template<int OtherSize>
	MC_HybridArray<Type, StaticSize>& operator=(const MC_HybridArray<Type, OtherSize>& anOther) = delete;

	Type& operator[](size_t anIndex)
	{
#if MC_HEAVY_DEBUG_GROWINGARRAY_BOUNDSCHECK
		MC_ASSERTMSG(anIndex >= 0 && anIndex < myCount, "MC_HybridArray BOUNDS ERROR!");
#endif

		return myData[anIndex];
	}

	const Type& operator[](size_t anIndex) const
	{
#if MC_HEAVY_DEBUG_GROWINGARRAY_BOUNDSCHECK
		MC_ASSERTMSG(anIndex >= 0 && anIndex < myCount, "MC_HybridArray BOUNDS ERROR!");
#endif

		return myData[anIndex];
	}

	Type *data()
	{
		return myData;
	}

	const Type *data() const
	{
		return myData;
	}

	Type *begin()
	{
		return myData;
	}

	const Type *begin() const
	{
		return myData;
	}

	Type *end()
	{
		return myData + myCount;
	}

	const Type *end() const
	{
		return myData + myCount;
	}

	bool empty() const
	{
		return size() == 0;
	}

	size_t size() const
	{
		return myCount;
	}

	void shrink_to_fit() = delete;

	void clear()
	{
		myCount = 0;
	}

	void push_back(const Type& anItem) = delete;
	void pop_back() = delete;
};
