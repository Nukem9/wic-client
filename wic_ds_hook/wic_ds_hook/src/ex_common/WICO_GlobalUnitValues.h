#pragma once

#pragma pack(push, 1)
class WICO_GlobalUnitValues
{
public:
	float	myGlobalFormationRadiusVariation;		 // this+0x0
	float	myGlobalUnitHealthMultiplier;		 // this+0x4
	float	myGlobalUnitScoreMultiplier;		 // this+0x8
	float	myGlobalUnitHealingScoreMultiplier;		 // this+0xC
	float	myGlobalUnitBridgeLayingScoreMultiplier;		 // this+0x10
	float	myGlobalUnitScoreToBattleHonorMultiplier;		 // this+0x14
	float	myGlobalReloadingTimeDelay;		 // this+0x18
	float	myGlobalUnitSeekingTurretSector;		 // this+0x1C
	float	myGlobalUnitSeekingTurretBaseTime;		 // this+0x20
	float	myGlobalUnitSeekingTurretRandomTime;		 // this+0x24
	float	myGlobalUnitTriggerProneRadius;		 // this+0x28
	int		myGlobalUnloadingMaxScore;		 // this+0x2C
	int		myGlobalUnloadingTime;		 // this+0x30
	int		myGlobalUnloadingMaxDistance;		 // this+0x34
	float	myGlobalUnitIconSpeedCap;		 // this+0x38
	float	myGlobalUnitIconLerpTimeModifier;		 // this+0x3C
	float	myGlobalRepairOwnRepairerMultiplier;		 // this+0x40
	float	myGlobalRepairOwnStandardUnitMultiplier;		 // this+0x44
	float	myGlobalMinimumSquadFormationSpreadOnSpawn;		 // this+0x48
	float	myGlobalInfantryFormationSpread;		 // this+0x4C
};
#pragma pack(pop)