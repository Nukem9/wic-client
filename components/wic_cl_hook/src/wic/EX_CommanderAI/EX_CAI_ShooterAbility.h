#pragma once

struct EX_CAI_ShooterAbility
{
	enum AbilityType : s32
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
		float myMinRange;
		float myMaxRange;
		bool myTargetCaps[3];
		bool myIsBallisticShooterFlag;
		bool myIsHomingShooterFlag;
		float myAccuracy;
		float myBlastRadius;
		float myFiringRate;
		u32 myMagazineCapacity;
		u32 myArmorPiercingDirectValue;
		u32 myArmorPiercingBlastValue;
		u32 myWeaponDamageDirect;
		u32 myWeaponDamageBlast;
		float myReloadTime;
		float myProjectileSpeed;
		float myShooterOffsetY;
	};
	static_assert_size(Shooter, 0x3C);

	class Healer
	{
	public:
		float myHealingValue;
	};
	static_assert_size(Healer, 0x4);

	class Cloud
	{
	public:
		bool myBlockLosFlag;
		bool myBlockRadarFlag;
		float myRadius;
	};
	static_assert_size(Cloud, 0x8);

	struct Support
	{
		bool myIsDeployableFlag;
		bool myIsDeployableMovmentFlag;
		bool myIsOffensiveFlag;
		s32 mySupportWeaponId;
		float myRange;
	};
	static_assert_size(Support, 0xC);

	struct ModeAbility
	{
		enum ModeType : s32
		{
			SPRINT,
			FIELDREPAIRS,
			NUM_ModeTypes,
		};

		ModeType myModeType;
	};
	static_assert_size(ModeAbility, 0x4);

	AbilityType myAbilityType;
	Shooter myShooter;
	Healer myHealer;
	Cloud myCloud;
	Support mySupport;
	ModeAbility myModeAbility;
	s32 mySpecialAbilityId;
};
static_assert_size(EX_CAI_ShooterAbility, 0x60);