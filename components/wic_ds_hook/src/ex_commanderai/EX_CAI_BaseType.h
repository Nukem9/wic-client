#pragma once

#include "../types.h"

class EXCO_UnitType;
class EX_CAI_ShooterAbility;
class EX_CAI_BaseTypeContainer;
class EX_CAI_BaseType;
class EX_CAI_Type;

#pragma pack(push, 1)
class EX_CAI_BaseType 
{
public:
	const uint			myId;			// this+0x0
	EXCO_UnitType&		myType;			// this+0x4
	EX_CAI_Type			*myUnitType;	// this+0x8
};
#pragma pack(pop)