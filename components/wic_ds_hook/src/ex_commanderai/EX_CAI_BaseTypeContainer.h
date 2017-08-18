#pragma once

class EX_CAI_ShooterAbility;
class EX_CAI_BaseTypeContainer;
class EX_CAI_BaseType;
class EX_CAI_Type;

#pragma pack(push, 1)
class EX_CAI_BaseTypeContainer 
{
	const unsigned int		myNumTypes;	// this+0x0
	const EX_CAI_BaseType	**myTypes;	// this+0x4
};
#pragma pack(pop)