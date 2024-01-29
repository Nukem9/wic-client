#pragma once

#include <MCommon2/MC_GrowingArray.h>
#include <MCommon2/MC_String.h>
#include "EX_CAI_ShooterAbility.h"

class EXCO_DamageModel;
class EXCO_UnitType;
class EX_CAI_BaseTypeContainer;

class EX_CAI_Type
{
public:
	enum EX_CAI_PathType : s32
	{
		EX_CAI_PATH_LAND,
		EX_CAI_PATH_LAND_AND_WOODS,
		EX_CAI_PATH_LAND_AND_WATER,
		EX_CAI_PATH_NONE,
		EX_CAI_PATH_NUMPATHS,
	};

	enum EX_CAI_TargettingType : s32
	{
		METATYPE_AIR,
		METATYPE_GROUND,
		METATYPE_INFANTRY,
		METATYPE_NUMTYPES,
	};

	s32 myDirectionalArmor[6];
	EXCO_DamageModel *myDamageModel;
	MC_GrowingArray<MC_String> mySquadMembers;
	EXCO_UnitType *myType;
	s32 myBaseTypeId;
	EX_CAI_ShooterAbility myOffensiveSpecialAbility;
	EX_CAI_ShooterAbility myDefensiveSpecialAbility;
	EX_CAI_BaseTypeContainer *myUnitTypes;
	bool myCanAttackFlag;
	bool myCanMoveFlag;
	bool myTargetCaps[3];
	EX_CAI_ShooterAbility::Shooter *myShooters[METATYPE_NUMTYPES];
	MC_GrowingArray<EX_CAI_ShooterAbility::Shooter>	myShooterData;
	EX_CAI_TargettingType myDefaultTargettingType;
	s32 myMaxDamageShooterIndex;
	bool myIsBallisticShooterFlag;
	bool myIsHomingShooterFlag;
	EX_CAI_PathType myAIPathType;
	float myMaxSpeed;
	float myViewRange;
	bool myIsZoneCapturerFlag;
	bool myIsResidentFlag;
	bool myIsRepairerFlag;
	bool myIsClaimableFlag;
	u32 myContainableType;
	u32 myContainerType;
	bool myIsBlowerFlag;
	float myBlastRadius;
	float myBlowerBlastRadius;
	float myRepairRadius;
	s32 myRepairedDamagePerTimeInterval;
	s32 myRepairIntervalInSeconds;
	s32 myContainerCargoCapacity;
	EX_CAI_TargettingType myTargetingType;
	float myOptimalRange;

	u32 GetWeaponDamageDirect() const;
	u32 GetWeaponDamageBlast() const;
	u32 GetArmorPiercingDirect() const;
	u32 GetArmorPiercingBlast() const;
	float GetFiringRate() const;
	u32 GetBulletsPerMag() const;
	float GetReloadTime() const;
	float GetAccuracy() const;
};
static_assert_size(EX_CAI_Type, 0x164);