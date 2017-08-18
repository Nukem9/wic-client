#pragma once

class EX_CAI_Troop_Request 
{
public:
	enum SupportWeaponTarget : int
	{
		TURTLE,
		PP_FORT,
		BRIDGE,
	};

	float				myNeededMobillity;					// this+0x0
	float				myNeededStrength;					// this+0x4
	float				myNeededDamageVsTargetingType[4];	// this+0x8
	float				myNumbersFactor;					// this+0x18
	int					myHardNumberLimit;					// this+0x1C
	float				myOpponentDamageVsTargetingType[4];	// this+0x20
	float				myTotalOpponentFirePower;			// this+0x30
	int					myCanCaptureNeed;					// this+0x34
	int					myFireSupportNeed;					// this+0x38
	int					myHealingSupportNeed;				// this+0x3C
	int					myAntiAirSupportNeed;				// this+0x40
	int					myTransporterNeed;					// this+0x44
	int					myAirToGroundNeed;					// this+0x48
	bool				myConnectivityNeed;					// this+0x4C
	bool				myBuyOnlyAmphOrAir;					// this+0x4D
	float				myNeededHealingNeed;				// this+0x50
	EX_CAI_Vector3f		myConnectivityPosition;				// this+0x54
	EX_CAI_Vector3f		mySupportWeaponFirePosition;		// this+0x60
	SupportWeaponTarget	mySupportWeaponTarget;				// this+0x6C
	int					mySupportWeaponRoleId;				// this+0x70
	float				myClosnessFactor;					// this+0x74
	unsigned int		myBridgeToRepair;					// this+0x78
};