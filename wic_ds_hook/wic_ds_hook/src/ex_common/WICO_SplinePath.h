#pragma once

#pragma pack(push, 1)
class WICO_SplinePath
{
	int				myRefCount;		 // this+0x0
	float			myTimeScaleValue;		 // this+0x4
	unsigned int	myKeySizeInFloats;		 // this+0x8
	unsigned int	myNumChannels;		 // this+0xC
	unsigned int	myChannelHasKeysBitMask;		 // this+0x10
	unsigned int	myNumTangents;		 // this+0x14
	float			*myDefaultChannelValues;		 // this+0x18
	float			*myKeys;		 // this+0x1C
	unsigned int	myNumKeys;		 // this+0x20
	bool			myIsLoopedFlag;		 // this+0x24
};
#pragma pack(pop)