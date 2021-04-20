#pragma once

template<typename T>
class MC_GrowingArray
{
public:
	int			myMaxNrOfItems;		// this+0x0
	uint32_t	myItemIncreaseSize;	// this+0x4
	uint32_t	mySafemodeFlag;		// this+0x8
	T			*myItemList;		// this+0xC

	T& At(int Index)
	{
		return myItemList[Index];
	}

	T& operator [](int Index)
	{
		return myItemList[Index];
	}
};

CHECK_OFFSET(MC_GrowingArray<int>, myMaxNrOfItems, 0x0);
CHECK_OFFSET(MC_GrowingArray<int>, myItemIncreaseSize, 0x4);
CHECK_OFFSET(MC_GrowingArray<int>, mySafemodeFlag, 0x8);
CHECK_OFFSET(MC_GrowingArray<int>, myItemList, 0xC);