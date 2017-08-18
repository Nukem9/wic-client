#pragma once

class MC_BitVector
{
public:
	class Proxy
	{
		unsigned char&	myValue;// this+0x0
		unsigned int	myMask;	// this+0x4
	};

	unsigned char	*myData;		// this+0x0
	unsigned int	myDataLength;	// this+0x4
	unsigned int	myNumIndices;	// this+0x8
};

CHECK_OFFSET(MC_BitVector, myData, 0x0);
CHECK_OFFSET(MC_BitVector, myDataLength, 0x4);
CHECK_OFFSET(MC_BitVector, myNumIndices, 0x8);