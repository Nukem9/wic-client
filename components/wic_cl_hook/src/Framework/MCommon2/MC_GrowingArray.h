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
#include "MC_Algorithm.h"
#include "MC_HeapImplementation.h"
#include "MC_Mem.h"

template <class Type>
class MC_StdComparer
{
public:
	static bool LessThan(const Type& anObj1, const Type& anObj2) { return (anObj1 < anObj2); }
	static bool Equals(const Type& anObj1, const Type& anObj2) { return (anObj1 == anObj2); }
};

template <class Type, class Type2>
class MC_StdComparer2
{
public:
	static bool LessThan(const Type& anObj1, const Type2& anObj2) { return (anObj1 < anObj2); }
	static bool Equals(const Type& anObj1, const Type2& anObj2) { return (anObj1 == anObj2); }
};

template<class Type>
class MC_GrowingArray
{
	MC_HEAPIMPLEMENTATION_REDEFINE_NEW;

protected:
	u32 myUsedNrOfItems = 0;
	u32 myMaxNrOfItems = 0;
	u32 myItemIncreaseSize : 31 = 0;
	u32 mySafemodeFlag : 1 = 1; // Used to determine if copy constructor should be used or not for items.
	Type* myItemList = nullptr;

public:
	MC_GrowingArray() = default;

	~MC_GrowingArray()
	{
		if (myItemList)
			Delete(myItemList);
	}

	MC_GrowingArray(const MC_GrowingArray& anOther) = delete;
	MC_GrowingArray<Type>& operator=(const MC_GrowingArray& anOther) = delete;

	Type& operator[](size_t anIndex)
	{
#if MC_HEAVY_DEBUG_GROWINGARRAY_BOUNDSCHECK
		MC_ASSERTMSG(anIndex >= 0 && anIndex < size(), "MC_GrowingArray BOUNDS ERROR!");
#endif

		return myItemList[anIndex];
	}

	const Type& operator[](size_t anIndex) const
	{
#if MC_HEAVY_DEBUG_GROWINGARRAY_BOUNDSCHECK
		MC_ASSERTMSG(anIndex >= 0 && anIndex < size(), "MC_GrowingArray BOUNDS ERROR!");
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
		return myItemList + myUsedNrOfItems;
	}

	const Type *end() const
	{
		return myItemList + myUsedNrOfItems;
	}

	bool empty() const
	{
		return size() == 0;
	}

	size_t size() const
	{
		return myUsedNrOfItems;
	}

	void shrink_to_fit() = delete;

	void clear()
	{
		myUsedNrOfItems = 0;
	}

	void push_back(const Type& anItem)
	{
		// Call Init() before using the array
		MC_ASSERT(myItemIncreaseSize > 0);

		if (myUsedNrOfItems == myMaxNrOfItems)
		{
			const auto newSize = myMaxNrOfItems + myItemIncreaseSize;

			if (SetSize(newSize) < newSize)
			{
				MC_ASSERTMSG(false, "MC_GrowingArray::push_back() - Out of memory!");
				__debugbreak();
			}
		}

		myItemList[myUsedNrOfItems] = anItem;
		myUsedNrOfItems++;
	}

	void pop_back() = delete;

	int FindInSortedArray(const Type& anItem) const
	{
		return FindInSortedArray2<MC_StdComparer<Type>, Type>(anItem);
	}

	// Returns index to anItem if exist in the array. Otherwise returns -1. Array must be sorted! Fast!
	template <class Comparer, class Compared>
	int FindInSortedArray2(const Compared& anItem) const
	{
		return MC_Algorithm::BinarySearch(myItemList, myItemList + myUsedNrOfItems, anItem, ComparerProxy<Comparer, Compared>());
	}

	// Needed to glue together old and new comparers
	template <typename COMPARER, typename COMPARED>
	struct ComparerProxy
	{
		inline bool LessThan(const Type& a, const COMPARED& b) const { return COMPARER::LessThan(a, b); }
		inline bool Equals(const Type& a, const COMPARED& b) const { return COMPARER::Equals(a, b); }
	};

	// Sorts the array using a Comparer object
	template <typename COMPARER>
	void Sort(const COMPARER& aComparer, u32 aFirstIndex, u32 aLastIndex, const bool aReverseFlag = false)
	{
		// Clamp first index to 0 (handles default param)
		if (aFirstIndex < 0)
			aFirstIndex = 0;

		// Clamp last index to last item (handles default param and values that are too large)
		if (aLastIndex < 0 || aLastIndex >= myUsedNrOfItems)
			aLastIndex = myUsedNrOfItems - 1;

		if (aLastIndex <= aFirstIndex)	// nothing to do?
			return;

		if (aReverseFlag)
			MC_Algorithm::SortReverse(myItemList + aFirstIndex, myItemList + aLastIndex + 1, aComparer);
		else
			MC_Algorithm::Sort(myItemList + aFirstIndex, myItemList + aLastIndex + 1, aComparer);
	}

	// Sorts the array using operator <
	void Sort(u32 aFirstIndex, u32 aLastIndex, const bool aReverseFlag = false)
	{
		Sort(MC_Algorithm::StandardComparer<Type>(), aFirstIndex, aLastIndex, aReverseFlag);
	}

	void Sort()
	{
		Sort(0, myUsedNrOfItems);
	}

	bool RemoveItemAtIndex(const u32 anItemNumber)
	{
#if MC_HEAVY_DEBUG_GROWINGARRAY_BOUNDSCHECK
		MC_ASSERTMSG(anItemNumber >= 0 && anItemNumber < myUsedNrOfItems, "MC_GrowingArray BOUNDS ERROR!");
#endif

		// Check bounds
		if (anItemNumber >= myUsedNrOfItems)
			return false;

		// Shift items
		for (auto i = anItemNumber + 1; i < myUsedNrOfItems; i++)
			myItemList[i - 1] = myItemList[i];

		myUsedNrOfItems--;
		return true;
	}

	void RemoveCyclicAtIndex(const u32 anItemNumber)
	{
		MC_ASSERT(anItemNumber < myUsedNrOfItems);

		myItemList[anItemNumber] = myItemList[myUsedNrOfItems - 1];
		myUsedNrOfItems--;
	}

private:
	Type* New(size_t aCount)
	{
#if MC_TEMP_MEMORY_HANDLER_ENABLED
		return new Type[aCount];
#else
		auto p = reinterpret_cast<Type *>(MC_TempAllocIfOwnerOnStack(this, aCount * sizeof(Type), __FILE__, __LINE__));

		for (size_t i = 0; i < aCount; i++)
			std::construct_at(p + i);

		return p;
#endif
	}

	void Delete(Type* aPointer)
	{
#if MC_TEMP_MEMORY_HANDLER_ENABLED
		delete[] aPointer;
#else
		std::destroy_n(begin(), size());
		MC_TempFree(aPointer);
#endif
	}

	u32 SetSize(u32 aSize)
	{
		if (aSize <= 0)
		{
			Delete(myItemList);
			myItemList = nullptr;
			myUsedNrOfItems = 0;
			myMaxNrOfItems = 0;

			return 0;
		}

		if (aSize < myUsedNrOfItems)
			aSize = myUsedNrOfItems;

		auto tempList = New(aSize);

		if (!tempList)
			return myMaxNrOfItems;

		// Copy the old elements to the new list
		if (myItemList)
		{
			if (mySafemodeFlag)
			{
				for (u32 i = 0; i < myUsedNrOfItems; i++)
					tempList[i] = myItemList[i];
			}
			else
			{
				// Use their horrible hack to copy the items without using the copy constructor
				memcpy(tempList, myItemList, myUsedNrOfItems * sizeof(Type));
			}

			Delete(myItemList);
		}

		myItemList = tempList;
		myMaxNrOfItems = aSize;

		return myMaxNrOfItems;
	}
};