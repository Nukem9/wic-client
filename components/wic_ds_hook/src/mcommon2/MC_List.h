#pragma once

template<typename T>
class MC_List
{
public:
	virtual void *__vecDelDtor(unsigned int);

	MC_Link<T>	*myFirstLink;	// this+0x4
	MC_Link<T>	*myLastLink;	// this+0x8
	int			myCount;		// this+0xC
};

CHECK_OFFSET(MC_List<int>, myFirstLink, 0x4);
CHECK_OFFSET(MC_List<int>, myLastLink, 0x8);
CHECK_OFFSET(MC_List<int>, myCount, 0xC);