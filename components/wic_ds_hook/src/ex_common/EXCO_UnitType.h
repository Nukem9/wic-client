#pragma once

#pragma pack(push, 1)
class EXCO_UnitType : public EXCO_Persistent<EXCO_UnitType>
{
	enum Order : int
	{
		ORDER_STOP,
		ORDER_ATTACKGROUND,
		ORDER_HOLDFIRE,
		ORDER_MOVE,
		ORDER_MOVE_BACKWARDS,
		ORDER_UNLOADALL,
		ORDER_REPAIR,
		ORDER_STACK,
		ORDER_FOLLOW,
		ORDER_LOAD,
		ORDER_ENTER_BUILDING,
		ORDER_BLOW,
		ORDER_LAYBRIDGE,
		ORDER_GO_TO_LANDINGZONE,
		ORDER_SELECT_LANDINGZONE,
		ORDER_SET_RALLYPOINT,
		ORDER_PLACE_REQUEST,
		ORDER_OFFENSIVE_SPECIAL_ATTACK,
		ORDER_DEFENSIVE_SPECIAL_ATTACK,
		ORDER_NONE,
		ORDER_NUMTYPES,
	};

	enum Role : int
	{
		ROLE_COMBAT,
		ROLE_FLANKER,
		ROLE_SCOUT,
		ROLE_SUPPORT,
		ROLE_CARRIER,
		ROLE_SPECIAL,
		ROLE_STATIONARY,
		ROLE_NUMTYPES,
	};

	enum MetaType : int
	{
		METATYPE_AIR,
		METATYPE_GROUND,
		METATYPE_INFANTRY,
		METATYPE_HEAVYARMOR,
		METATYPE_NUMTYPES,
	};

	enum MetaCategory : int
	{
		METACATEGORY_INFANTRY,
		METACATEGORY_VEHICLE,
		METACATEGORY_TANK,
		METACATEGORY_COPTER,
		METACATEGORY_MISC,
		METACATEGORY_DEEP_WATER,
		METACATEGORY_NONTARGETABLE,
		METACATEGORY_NUMTYPES,
	};

	enum RoleCostRebate : int
	{
		REBATE_UNIQUE,
		REBATE_REBATED,
		REBATE_NORMAL,
		REBATE_PREMIUM,
		REBATE_LIMITED,
		REBATE_NUMTYPES,
	};

	class RoleCost 
	{
	public:
		int				roleName;	// this+0x0
		int				price;		// this+0x4
		RoleCostRebate	priceType;	// this+0x8
	};

	enum CombinedType : int
	{
		COMBINEDTYPE_HEAVYARMOR,
		COMBINEDTYPE_MEDIUMARMOR,
		COMBINEDTYPE_ARTILLERY,
		COMBINEDTYPE_HELI,
		COMBINEDTYPE_ATTACKHELI,
		COMBINEDTYPE_INFANTRY,
		COMBINEDTYPE_ANTIAIR,
		COMBINEDTYPE_ENGINEER,
		COMBINEDTYPE_TROOPTRANSPORT,
		COMBINEDTYPE_NUMTYPES,
	};

	float					myDirectionalArmorSlopes[6];	// this+0x1C
	int						myDirectionalArmor[6];			// this+0x34
	const int				myName;							// this+0x4C
	int						myWeight;						// this+0x50
	int						myUserData;						// this+0x54
	MC_GrowingArray<int>	mySoundIds;						// this+0x58

	class Experience
	{
	public:
		unsigned int	myExperienceLimit;		 // this+0x0
		float			myLeadPercentageMultiplier;		 // this+0x4
		float			myDevianceMultiplier;		 // this+0x8
		float			myMovementPenaltyMultiplier;		 // this+0xC
		float			myReloadTimeMultiplier;		 // this+0x10
		float			myViewRangeMultiplier;		 // this+0x14
		float			myMaxSpeedMultiplier;		 // this+0x18
		float			myRepairTimeIntervalMultiplier;		 // this+0x1C
	};

	WICO_CollisionObject_Instance	myCollisionObject;		 // this+0x68
	int								myTeam;		 // this+0xC0
	unsigned int					myMaterialID;		 // this+0xC4
	float							myRadius;		 // this+0xC8
	float							myRadius2;		 // this+0xCC
	float							mySelectionRadius;		 // this+0xD0
	float							myWaterTrailWidth;		 // this+0xD4
	int								myMaxHealth;		 // this+0xD8
	int								myMaintenance;		 // this+0xDC
	int								myPresence;		 // this+0xE0
	unsigned int					myCost;		 // this+0xE4
	unsigned int					myScore;		 // this+0xE8
	int								myUnitFeedbackType;		 // this+0xEC
	const Ice						*myDeathEffects;		 // this+0xF0
	unsigned int					mySpawnerOccupation;		 // this+0xF4
	Role							myRoleType;		 // this+0xF8
	MetaType						myMetaType;		 // this+0xFC
	MetaCategory					myCategory;		 // this+0x100
	CombinedType					myCombinedType;		 // this+0x104
	bool							myDisableWhenKilledFlag;		 // this+0x108
	const Ice						*myRep;		 // this+0x10C
	const Ice						*myParasites;		 // this+0x110
	Order							myOrderSlots[11];		 // this+0x114
	const char						*myOrderSlotIconFiles[11];		 // this+0x140
	bool							myImmuneVsDamages;		 // this+0x16C
	bool							myIsSelectableFlag;		 // this+0x16D
	MC_GrowingArray<int>			myPlayerRoleBlackList;		 // this+0x170
	MC_GrowingArray<RoleCost>		myPlayerRoleCosts;		 // this+0x180
	EXCO_DamageModel&				myDamageModel;		 // this+0x190
	WICO_GlobalUnitValues&			myGlobalUnitValues;		 // this+0x194
	unsigned int					myNumExperienceLevels;		 // this+0x198
	Experience						myExperienceLevels[5];		 // this+0x19C
	int								myUnitTypeId;		 // this+0x23C
	MC_Str<wchar_t, 0>				myUIName;		 // this+0x240
	MC_Str<wchar_t, 0>				myToolTipInfo;		 // this+0x244
	const char						*myIconWorldPrefab;		 // this+0x248
	const char						*myIconMenuPrefab;		 // this+0x24C
	const char						*myIconToolTipBitmap;		 // this+0x250
	const char						*myDefaultState;		 // this+0x254
	bool							myIsWriteProtected;		 // this+0x258
	const Ice						*myIce;		 // this+0x25C
	unsigned int					myNumLimitedPurcasableLeft;		 // this+0x260
	MC_Str<char, 0>					mySpawnModelPath;		 // this+0x264
	MC_GrowingArray<WICO_SplinePath *>	mySpawnModelSplines;		 // this+0x268
	EXCO_SoundSourceType			mySpawnModelSoundSource;		 // this+0x278
	WICAI_PathType					myPathType;		 // this+0x284
};
#pragma pack(pop)