#pragma once

class IceLocDecoder
{
public:
	uint32_t	myNumStrings;	// this+0x0
	wchar_t		**myPtrs;		// this+0x4
};

CHECK_OFFSET(IceLocDecoder, myNumStrings, 0x0);
CHECK_OFFSET(IceLocDecoder, myPtrs, 0x4);