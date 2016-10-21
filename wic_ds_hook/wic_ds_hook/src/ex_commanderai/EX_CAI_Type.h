#pragma once

class EXCO_DamageModel;
class EXCO_UnitType;
class EX_CAI_ShooterAbility;
class EX_CAI_BaseTypeContainer;
class EX_CAI_BaseType;
class EX_CAI_Type;

class EX_CAI_Type 
{
public:
	enum EX_CAI_PathType
	{
		EX_CAI_PATH_LAND,
		EX_CAI_PATH_LAND_AND_WOODS,
		EX_CAI_PATH_LAND_AND_WATER,
		EX_CAI_PATH_NONE,
		EX_CAI_PATH_NUMPATHS,
	};

	enum EX_CAI_TargettingType
	{
		METATYPE_AIR,
		METATYPE_GROUND,
		METATYPE_INFANTRY,
		METATYPE_NUMTYPES,
	};

	int								myDirectionalArmor[6];		// this+0x0
	EXCO_DamageModel				*myDamageModel;				// this+0x18
	MC_GrowingArray<MC_Str<char,0>>	mySquadMembers;				// this+0x1C
	EXCO_UnitType					*myType;					// this+0x2C
	int								myBaseTypeId;				// this+0x30
	EX_CAI_ShooterAbility			myOffensiveSpecialAbility;	// this+0x34
	EX_CAI_ShooterAbility			myDefensiveSpecialAbility;	// this+0x94
	EX_CAI_BaseTypeContainer		*myUnitTypes;				// this+0xF4
	bool							myCanAttackFlag;			// this+0xF8
	bool							myCanMoveFlag;				// this+0xF9
	bool							myTargetCaps[3];			// this+0xFA
	EX_CAI_ShooterAbility::Shooter	*myShooters[3];				// this+0x100
	MC_GrowingArray<EX_CAI_ShooterAbility::Shooter>	myShooterData;// this+0x10C
	EX_CAI_TargettingType			myDefaultTargettingType;	// this+0x11C
	int								myMaxDamageShooterIndex;	// this+0x120
	bool							myIsBallisticShooterFlag;	// this+0x124
	bool							myIsHomingShooterFlag;		// this+0x125
	EX_CAI_PathType					myAIPathType;				// this+0x128
	float							myMaxSpeed;					// this+0x12C
	float							myViewRange;				// this+0x130
	bool							myIsZoneCapturerFlag;		// this+0x134
	bool							myIsResidentFlag;			// this+0x135
	bool							myIsRepairerFlag;			// this+0x136
	bool							myIsClaimableFlag;			// this+0x137
	unsigned int					myContainableType;			// this+0x138
	unsigned int					myContainerType;			// this+0x13C
	bool							myIsBlowerFlag;				// this+0x140
	float							myBlastRadius;				// this+0x144
	float							myBlowerBlastRadius;		// this+0x148
	float							myRepairRadius;				// this+0x14C
	int								myRepairedDamagePerTimeInterval;// this+0x150
	int								myRepairIntervalInSeconds;	// this+0x154
	int								myContainerCargoCapacity;	// this+0x158
	EX_CAI_TargettingType			myTargetingType;			// this+0x15C
	float							myOptimalRange;				// this+0x160

	static void InitializeHook()
	{
		Detours::X86::DetourFunctionClass((PBYTE)0x006CA630, &GetWeaponDamageDirect);
		Detours::X86::DetourFunctionClass((PBYTE)0x006CA6C0, &GetWeaponDamageBlast);
		Detours::X86::DetourFunctionClass((PBYTE)0x006CA750, &GetArmorPiercingDirect);
		Detours::X86::DetourFunctionClass((PBYTE)0x006CA7E0, &GetArmorPiercingBlast);
		Detours::X86::DetourFunctionClass((PBYTE)0x006CA870, &GetFiringRate);
		Detours::X86::DetourFunctionClass((PBYTE)0x006CA900, &GetBulletsPerMag);
		Detours::X86::DetourFunctionClass((PBYTE)0x006CA990, &GetReloadTime);
		Detours::X86::DetourFunctionClass((PBYTE)0x006CAA20, &GetAccuracy);
	}

	const unsigned int GetWeaponDamageDirect()
	{
		int index = this->myMaxDamageShooterIndex;

		if(index == -1)
			index = 0;

		return this->myShooterData[index].myWeaponDamageDirect;
	}

	const unsigned int GetWeaponDamageBlast()
	{
		int index = this->myMaxDamageShooterIndex;

		if(index == -1)
			index = 0;

		return this->myShooterData[index].myWeaponDamageBlast;
	}

	const unsigned int GetArmorPiercingDirect()
	{
		int index = this->myMaxDamageShooterIndex;

		if(index == -1)
			index = 0;

		return this->myShooterData[index].myArmorPiercingDirectValue;
	}

	const unsigned int GetArmorPiercingBlast()
	{
		int index = this->myMaxDamageShooterIndex;

		if(index == -1)
			index = 0;

		return this->myShooterData[index].myArmorPiercingBlastValue;
	}

	const float GetFiringRate()
	{
		int index = this->myMaxDamageShooterIndex;

		if(index == -1)
			index = 0;

		return this->myShooterData[index].myFiringRate;
	}

	const unsigned int GetBulletsPerMag()
	{
		int index = this->myMaxDamageShooterIndex;

		if(index == -1)
			index = 0;

		return this->myShooterData[index].myMagazineCapacity;
	}

	const float GetReloadTime()
	{
		int index = this->myMaxDamageShooterIndex;

		if(index == -1)
			index = 0;

		return this->myShooterData[index].myReloadTime;
	}

	const float GetAccuracy()
	{
		int index = this->myMaxDamageShooterIndex;

		if(index == -1)
			index = 0;

		return this->myShooterData[index].myAccuracy;
	}
};

CHECK_OFFSET(EX_CAI_Type, myDirectionalArmor, 0x0);
CHECK_OFFSET(EX_CAI_Type, myDamageModel, 0x18);
CHECK_OFFSET(EX_CAI_Type, mySquadMembers, 0x1C);
CHECK_OFFSET(EX_CAI_Type, myType, 0x2C);
CHECK_OFFSET(EX_CAI_Type, myBaseTypeId, 0x30);
CHECK_OFFSET(EX_CAI_Type, myOffensiveSpecialAbility, 0x34);
CHECK_OFFSET(EX_CAI_Type, myDefensiveSpecialAbility, 0x94);
CHECK_OFFSET(EX_CAI_Type, myUnitTypes, 0xF4);
CHECK_OFFSET(EX_CAI_Type, myCanAttackFlag, 0xF8);
CHECK_OFFSET(EX_CAI_Type, myCanMoveFlag, 0xF9);
CHECK_OFFSET(EX_CAI_Type, myTargetCaps, 0xFA);
CHECK_OFFSET(EX_CAI_Type, myShooters, 0x100);
CHECK_OFFSET(EX_CAI_Type, myShooterData, 0x10C);
CHECK_OFFSET(EX_CAI_Type, myDefaultTargettingType, 0x11C);
CHECK_OFFSET(EX_CAI_Type, myMaxDamageShooterIndex, 0x120);
CHECK_OFFSET(EX_CAI_Type, myIsBallisticShooterFlag, 0x124);
CHECK_OFFSET(EX_CAI_Type, myIsHomingShooterFlag, 0x125);
CHECK_OFFSET(EX_CAI_Type, myAIPathType, 0x128);
CHECK_OFFSET(EX_CAI_Type, myMaxSpeed, 0x12C);
CHECK_OFFSET(EX_CAI_Type, myViewRange, 0x130);
CHECK_OFFSET(EX_CAI_Type, myIsZoneCapturerFlag, 0x134);
CHECK_OFFSET(EX_CAI_Type, myIsResidentFlag, 0x135);
CHECK_OFFSET(EX_CAI_Type, myIsRepairerFlag, 0x136);
CHECK_OFFSET(EX_CAI_Type, myIsClaimableFlag, 0x137);
CHECK_OFFSET(EX_CAI_Type, myContainableType, 0x138);
CHECK_OFFSET(EX_CAI_Type, myContainerType, 0x13C);
CHECK_OFFSET(EX_CAI_Type, myIsBlowerFlag, 0x140);
CHECK_OFFSET(EX_CAI_Type, myBlastRadius, 0x144);
CHECK_OFFSET(EX_CAI_Type, myBlowerBlastRadius, 0x148);
CHECK_OFFSET(EX_CAI_Type, myRepairRadius, 0x14C);
CHECK_OFFSET(EX_CAI_Type, myRepairedDamagePerTimeInterval, 0x150);
CHECK_OFFSET(EX_CAI_Type, myRepairIntervalInSeconds, 0x154);
CHECK_OFFSET(EX_CAI_Type, myContainerCargoCapacity, 0x158);
CHECK_OFFSET(EX_CAI_Type, myTargetingType, 0x15C);
CHECK_OFFSET(EX_CAI_Type, myOptimalRange, 0x160);