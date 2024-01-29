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

void MC_HashMap_HashData(const void* aData, s32 aByteCount, s32 aHashSize, u32* aDest, u32* aSeed = 0);
u32 MC_HashMap_HashData(const void* aData, s32 aByteCount, u32 aSeed = 0);
u32 MC_HashMap_HashString(const void* aData, u32 aSeed = 0);

// Type helpers
template<class T>
inline u32 MC_HashMap_GetHash(const T& anItem)
{
	return MC_HashMap_HashData((const char*)&anItem, sizeof(anItem));
}

template<>
inline u32 MC_HashMap_GetHash(const char* const &anItem)
{
	return MC_HashMap_HashString(anItem);
}

template<class T>
inline bool MC_HashMap_Compare(const T& aA, const T& aB)
{
	return (aA == aB);
}

template<>
inline bool MC_HashMap_Compare(const char* const &aA, const char* const &aB)
{
	return strcmp(aA, aB) == 0;
}

template<class T>
inline void MC_HashMap_Set(T& aDest, const T& aSrc)
{
	aDest = aSrc;
}

template<>
inline void MC_HashMap_Set(const char* &aDest, const char* const &aSrc)
{
	delete[] aDest;
	aDest = nullptr;

	if (aSrc)
	{
		const auto bufSize = strlen(aSrc) + 1;
		const auto copy = new char[bufSize];

		for (size_t i = 0; i < bufSize; i++)
			copy[i] = aSrc[i];

		aDest = copy;
	}
}

template<class T>
inline void MC_HashMap_Clear(T& anItem)
{
}

template<>
inline void MC_HashMap_Clear(const char* &anItem)
{
	anItem = nullptr;
}

template<class T>
inline void MC_HashMap_Destroy(T& anItem)
{
}

template<>
inline void MC_HashMap_Destroy(const char* &anItem)
{
	delete[] anItem;
	anItem = nullptr;
}

// The hash map
template<class KEY, class ITEM, class DERIVED>
class MC_HashBase
{
	struct Entry
	{
		Entry()
		{
			MC_HashMap_Clear(myKey);
		}

		~Entry()
		{
			MC_HashMap_Destroy(myKey);
		}

		ITEM myItem;
		KEY myKey;
		bool myValidFlag = false;
	};

public:
	typedef KEY key_type;
	typedef ITEM item_type;

	class Iterator
	{
	public:
		Iterator() = default;
		Iterator(const Iterator& anOther) : myMap(anOther.myMap), myIndex(anOther.myIndex) {}
		Iterator(MC_HashBase& aMap) : myMap(&aMap) { for (myIndex = 0; myIndex < myMap->myArraySize && !myMap->myEntries[myIndex].myValidFlag; ++myIndex) {} }
		Iterator(MC_HashBase* aMap) : myMap(aMap) { for (myIndex = 0; myIndex < myMap->myArraySize && !myMap->myEntries[myIndex].myValidFlag; ++myIndex) {} }

		Iterator& operator=(const Iterator& anOther)
		{
			myMap = anOther.myMap;
			myIndex = anOther.myIndex;
			
			return *this;
		}

		operator bool() const { return myMap && (myIndex < myMap->myArraySize); }
		const KEY&		Key() const { return myMap->myEntries[myIndex].myKey; }
		ITEM&			Item() { return myMap->myEntries[myIndex].myItem; }
		const ITEM&		Item() const { return myMap->myEntries[myIndex].myItem; }
		ITEM*			operator ->() { return &myMap->myEntries[myIndex].myItem; }
		const ITEM*		operator ->() const { return &myMap->myEntries[myIndex].myItem; }

		// ++ and -- operators returns void to avoid prefix/postfix problems.
		void	operator++() { for (++myIndex; myIndex < myMap->myArraySize && !myMap->myEntries[myIndex].myValidFlag; ++myIndex) {} }	// prefix
		void	operator++(int dummy) { ++*this; }		// postfix
		void	operator--() { for (--myIndex; myIndex < myMap->myArraySize && !myMap->myEntries[myIndex].myValidFlag; --myIndex) {} }	// prefix
		void	operator--(int dummy) { --*this; }		// postfix

	private:
		MC_HashBase *myMap = nullptr;
		u32 myIndex = 0;
	};

	Iterator Begin()
	{
		return Iterator(*this);
	}

	MC_HashBase(const s32 aStartSize)
	{
		myArraySize = aStartSize > 3 ? aStartSize : 3;
		myEntries = new Entry[myArraySize];
		myNumEntries = 0;
	}

	~MC_HashBase()
	{
		delete[] myEntries;
	}

	s32 Count() const
	{
		return s32(myNumEntries);
	}

	ITEM& operator[](const KEY& aKey)
	{
		u32 i;

		if (!GetIndex(aKey, i, myArraySize, myEntries))
		{
			// Grow if necessary
			if (myNumEntries > myArraySize / 2)
			{
				Resize((myArraySize + 1) * 2 - 1);
				GetIndex(aKey, i, myArraySize, myEntries);
			}

			myEntries[i].myValidFlag = true;

			MC_HashMap_Set(myEntries[i].myKey, aKey);
			myNumEntries++;
		}

		return myEntries[i].myItem;
	}

	bool FindKeyByItem(const ITEM& anItem, KEY& aKey) const
	{
		for (s32 i = 0; i < myArraySize; i++)
		{
			if (myEntries[i].myValidFlag)
			{
				if (MC_HashMap_Compare(myEntries[i].myItem, anItem))
				{
					aKey = myEntries[i].myKey;
					return true;
				}
			}
		}

		return false;
	}

	bool RemoveByKey(const KEY& aKey, bool aReallocOnShrink = true)
	{
		if (u32 i; GetIndex(aKey, i, myArraySize, myEntries))
		{
			RemoveAtIndex(i, aReallocOnShrink);
			return true;
		}

		return false;
	}

	bool PopByKey(const KEY& aKey, ITEM& anItem, bool aReallocOnShrink = true)
	{
		if (u32 i; GetIndex(aKey, i, myArraySize, myEntries))
		{
			anItem = myEntries[i].myItem;
			RemoveAtIndex(i, aReallocOnShrink);
			return true;
		}

		return false;
	}

	// Removes all entries with the specified item
	int RemoveByItem(const ITEM& anItem, bool aReallocOnShrink = true)
	{
		s32 count = 0;

		for (u32 i = 0; i < myArraySize; i++)
		{
			if (myEntries[i].myValidFlag)
			{
				if (MC_HashMap_Compare(myEntries[i].myItem, anItem))
				{
					RemoveAtIndex(i, aReallocOnShrink);
					count++;
					i = -1;		// start over (the for loop will increment this to 0)
				}
			}
		}

		return count;
	}

	void RemoveAll(bool aShrinkBufferFlag = true)
	{
		for (u32 i = 0; i < myArraySize; i++)
		{
			if (myEntries[i].myValidFlag)
			{
				myEntries[i].myValidFlag = false;
				MC_HashMap_Destroy(myEntries[i].myKey);
			}
		}

		myNumEntries = 0;

		// Only shrink if it's worth while and if we want to
		if (aShrinkBufferFlag && myArraySize > 6)
		{
			delete[] myEntries;
			myArraySize = 3;
			myEntries = new Entry[myArraySize];
		}
	}

	void DeleteAll(bool aShrinkBufferFlag = true)
	{
		for (u32 i = 0; i < myArraySize; i++)
		{
			if (myEntries[i].myValidFlag)
			{
				myEntries[i].myValidFlag = false;
				delete myEntries[i].myItem;
				myEntries[i].myItem = nullptr;
				MC_HashMap_Destroy(myEntries[i].myKey);
			}
		}

		myNumEntries = 0;

		// Only shrink if it's worth while and if we want to
		if (aShrinkBufferFlag && myArraySize > 6)
		{
			delete[] myEntries;
			myArraySize = 3;
			myEntries = new Entry[myArraySize];
		}
	}

	bool Exists(const KEY& aKey) const
	{
		u32 i;
		return GetIndex(aKey, i, myArraySize, myEntries);
	}

	bool GetIfExists(const KEY& aKey, ITEM& anItem) const
	{
		if (u32 i; GetIndex(aKey, i, myArraySize, myEntries))
		{
			anItem = myEntries[i].myItem;
			return true;
		}

		return false;
	}

	ITEM* GetIfExists(const KEY& aKey) const
	{
		if (u32 i; GetIndex(aKey, i, myArraySize, myEntries))
			return &myEntries[i].myItem;

		return 0;
	}

	// This function is slow and should be avoided if possible
	KEY* GetKeyByIndexNumber(s32 aIndex) const
	{
		s32 numFound = 0;
		for (u32 a = 0; a < myArraySize; ++a)
		{
			if (myEntries[a].myValidFlag)
			{
				++numFound;
				if (numFound > aIndex)
					return &myEntries[a].myKey;
			}
		}

		return 0;
	}

private:
	bool GetIndex(const KEY& aKey, u32& i, u32 maxEntries, Entry* pEntries) const
	{
		i = ((DERIVED*)this)->GetHash(aKey) % maxEntries;
		const auto starti = i;

		while (pEntries[i].myValidFlag)
		{
			if (MC_HashMap_Compare(aKey, pEntries[i].myKey))
				return true;

			i = (i + 1) % maxEntries;

			if (i == starti)
				return false;
		}

		return false;
	}

	void RemoveAtIndex(u32 anIndex, bool aReallocOnShrink)
	{
		myNumEntries--;
		assert(myEntries[anIndex].myValidFlag);
		myEntries[anIndex].myValidFlag = false;
		MC_HashMap_Destroy(myEntries[anIndex].myKey);

		// Shrink if necessary (and if we want to)
		if (aReallocOnShrink && myNumEntries < myArraySize / 5)
		{
			Resize(((myArraySize + 1) / 2) - 1);
		}
		else
		{
			// Fill hole that might occur
			u32 i = anIndex;
			// Start at the next item in the array, and loop until the first hole
			for (i = (i + 1) % myArraySize; myEntries[i].myValidFlag; i = (i + 1) % myArraySize)
			{
				// Calc the index that this items wants to reside in
				unsigned int j = ((DERIVED*)this)->GetHash(myEntries[i].myKey) % myArraySize;

				// Run j util we find the first hole
				while (i != j && myEntries[j].myValidFlag)
				{
					j = (j + 1) % myArraySize;
				}

				if (i != j)
				{
					myEntries[j].myKey = myEntries[i].myKey;
					myEntries[j].myItem = myEntries[i].myItem;
					myEntries[j].myValidFlag = true;
					MC_HashMap_Clear(myEntries[i].myKey);
					myEntries[i].myValidFlag = false;
				}
			}
		}
	}

	void Resize(u32 aNewMaxSize)
	{
		Entry* pNewEntries = new Entry[aNewMaxSize];

		for (u32 readIndex = 0; readIndex < myArraySize; readIndex++)
		{
			if (myEntries[readIndex].myValidFlag)
			{
				unsigned int i;
				GetIndex(myEntries[readIndex].myKey, i, aNewMaxSize, pNewEntries);
				pNewEntries[i].myKey = myEntries[readIndex].myKey;
				pNewEntries[i].myItem = myEntries[readIndex].myItem;
				pNewEntries[i].myValidFlag = true;
				MC_HashMap_Clear(myEntries[readIndex].myKey);
			}
		}

		delete[] myEntries;
		myEntries = pNewEntries;
		myArraySize = aNewMaxSize;
	}

	Entry *myEntries = nullptr;
	u32 myArraySize = 0;
	u32 myNumEntries = 0;
};

template<class KEY, class ITEM>
class MC_HashMap : public MC_HashBase<KEY, ITEM, MC_HashMap<KEY, ITEM>>
{
public:
	using BASE = typename MC_HashBase<KEY, ITEM, MC_HashMap<KEY, ITEM>>;
	friend class BASE;

	explicit MC_HashMap(const s32 aStartSize = 7)
		: BASE(aStartSize)
	{}

private:
	// Required by MC_HashBase
	u32 GetHash(const typename BASE::key_type& aKey)
	{
		return MC_HashMap_GetHash<KEY>(aKey);
	}
};

template<class ITEM>
class MC_HashSet : public MC_HashBase<u32, ITEM, MC_HashSet<ITEM>>
{
public:
	using BASE = typename MC_HashBase<u32, ITEM, MC_HashSet<ITEM>>;
	friend class BASE;

	explicit MC_HashSet(const s32 aStartSize = 7)
		: BASE(aStartSize)
	{}

private:
	// Required by MC_HashBase
	u32 GetHash(const typename BASE::key_type& aKey)
	{
		return aKey;
	}
};