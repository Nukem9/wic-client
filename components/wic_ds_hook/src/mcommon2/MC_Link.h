#pragma once

template<typename T>
class MC_Link
{
public:
	T			myValue;	// this+0x0
	MC_Link<T> *myNextLink;	// this+0x4
	MC_Link<T> *myPrevLink;	// this+0x8
};

CHECK_OFFSET(MC_Link<int>, myValue, 0x0);
CHECK_OFFSET(MC_Link<int>, myNextLink, 0x4);
CHECK_OFFSET(MC_Link<int>, myPrevLink, 0x8);