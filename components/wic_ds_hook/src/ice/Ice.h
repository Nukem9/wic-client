#pragma once

#include "IceLocDecoder.h"

#pragma pack(push, 1)
class Ice
{
public:
	enum FailureMode
	{
		AssertOnFail,
		WarnOnFail,
		DontWarnOnFail,
	};

	class ReturnProxy
	{
		const Ice *myIce;// this+0x0
	};

	Ice				*myParent;		// this+0x0
	IceLocDecoder	*myLocDecoder;	// this+0x4

	union
	{
		char		*myValue;		// this+0x8
		const Ice	*myReference;	// this+0x8
		Ice			*myMembers;		// this+0x8
	};

	int		myTypeName;			// this+0xC
	int		myInstanceName;		// this+0x10
	int		myIceType;			// this+0x14
	char	*myDataBuf;			// this+0x18
	char	*myName;			// this+0x1C
};

CHECK_OFFSET(Ice, myParent, 0x0);
CHECK_OFFSET(Ice, myLocDecoder, 0x4);
CHECK_OFFSET(Ice, myValue, 0x8);
CHECK_OFFSET(Ice, myTypeName, 0xC);
CHECK_OFFSET(Ice, myInstanceName, 0x10);
CHECK_OFFSET(Ice, myIceType, 0x14);
CHECK_OFFSET(Ice, myDataBuf, 0x18);
CHECK_OFFSET(Ice, myName, 0x1C);
#pragma pack(pop)