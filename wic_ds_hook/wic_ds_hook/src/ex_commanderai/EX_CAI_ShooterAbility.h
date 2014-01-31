#pragma once

class EX_CAI_ShooterAbility 
{
public:
	enum AbilityType : int
	{
		HEALER,
		SUPPORT,
		SHOOTER,
		CLOUD,
		MODEABILITY,
		NUM_ABILITYTYPES,
	};

	class Shooter 
	{
	public:
		float			myMinRange;					// this+0x0
		float			myMaxRange;					// this+0x4
		bool			myTargetCaps[3];			// this+0x8
		bool			myIsBallisticShooterFlag;	// this+0xB
		bool			myIsHomingShooterFlag;		// this+0xC
		//char _pad1[3];								// this+0xD
		float			myAccuracy;					// this+0x10
		float			myBlastRadius;				// this+0x14
		float			myFiringRate;				// this+0x18
		unsigned int	myMagazineCapacity;			// this+0x1C
		unsigned int	myArmorPiercingDirectValue;	// this+0x20
		unsigned int	myArmorPiercingBlastValue;	// this+0x24
		unsigned int	myWeaponDamageDirect;		// this+0x28
		unsigned int	myWeaponDamageBlast;		// this+0x2C
		float			myReloadTime;				// this+0x30
		float			myProjectileSpeed;			// this+0x34
		float			myShooterOffsetY;			// this+0x38
	};

	class Healer 
	{
	public:
		float myHealingValue;// this+0x0
	};

	class Cloud 
	{
	public:
		bool	myBlockLosFlag;		// this+0x0
		bool	myBlockRadarFlag;	// this+0x1
		//char _pad1[2];				// this+0x2
		float	myRadius;			// this+0x4
	};

	class Support 
	{
	public:
		bool	myIsDeployableFlag;			// this+0x0
		bool	myIsDeployableMovmentFlag;	// this+0x1
		bool	myIsOffensiveFlag;			// this+0x2
		//char _pad1[1];						// this+0x3
		int		mySupportWeaponId;			// this+0x4
		float	myRange;					// this+0x8
	};

	class ModeAbility 
	{
	public:
		enum ModeType : int
		{
			SPRINT,
			FIELDREPAIRS,
			NUM_ModeTypes,
		};

		ModeType myModeType;// this+0x0
	};

	AbilityType	myAbilityType;		// this+0x0
	Shooter		myShooter;			// this+0x4
	Healer		myHealer;			// this+0x40
	Cloud		myCloud;			// this+0x44
	Support		mySupport;			// this+0x4C
	ModeAbility	myModeAbility;		// this+0x58
	int			mySpecialAbilityId;	// this+0x5C
};

CHECK_OFFSET(EX_CAI_ShooterAbility, myAbilityType, 0x0);
CHECK_OFFSET(EX_CAI_ShooterAbility, myShooter, 0x4);
CHECK_OFFSET(EX_CAI_ShooterAbility, myHealer, 0x40);
CHECK_OFFSET(EX_CAI_ShooterAbility, myCloud, 0x44);
CHECK_OFFSET(EX_CAI_ShooterAbility, mySupport, 0x4C);
CHECK_OFFSET(EX_CAI_ShooterAbility, myModeAbility, 0x58);
CHECK_OFFSET(EX_CAI_ShooterAbility, mySpecialAbilityId, 0x5C);