#pragma once

template<typename T, int MaxSize>
class MC_HybridArray
{
public:
	int myCount;				// this+0x00
	int myMaxSize;				// this+0x04
	T	*myData;				// this+0x08
	T	myStaticData[MaxSize];	// this+0x0C

	MC_HybridArray()
	{
		this->myCount	= 0;
		this->myMaxSize = MaxSize;

		this->myData = nullptr;
	}

	void RemoveAtIndex(int aIndex)
	{
		MC_ASSERT(aIndex >= 0 && aIndex < myCount);

		// Shift each element over
		for (int i = aIndex; i < this->myCount - 1; i++)
			this->myData[i] = this->myData[i + 1];

		// Decrease total element count
		this->myCount--;
	}

	T& operator [] (int aIndex)
	{
		MC_ASSERT(aIndex >= 0 && aIndex < myCount);

		return this->myData[aIndex];
	}
};