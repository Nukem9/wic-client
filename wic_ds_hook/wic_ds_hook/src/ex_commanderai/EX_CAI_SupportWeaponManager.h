#pragma once

class EX_CAI_SupportWeaponManager : public EX_CAI_Manager
{
public:
	EX_CAI_GameWrapper		*myGameWrapper;			// this+0xC
	float					myValue;				// this+0x10
	int						myApCost;				// this+0x14
	char					myTypeName[255];		// this+0x18
	float					myReloadTime;			// this+0x118
	float					myFireTime;				// this+0x11C
	MC_Vector3<float>		myFiringPosition;		// this+0x120
	MC_Vector2<float>		myFiringDirection;		// this+0x12C
	float					myFiringFitness;		// this+0x134
	EX_CAI_CommanderAI_Data	*myCommanderAiData;		// this+0x138
	bool					myIsSleepingFlag;		// this+0x13C
	bool					myExitedSleepFlag;		// this+0x13D
	bool					myFirstTimeFlag;		// this+0x13E
	bool					myIsBridgeRepairer;		// this+0x13F

	static void InitializeHook()
	{
		Detours::X86::DetourClassFunction((PBYTE)0x0072FA60, &NonVirt_Update);
	}

	void NonVirt_Update()
	{
		if(this->myFirstTimeFlag)
		{
			this->myFireTime = MI_Time::ourCurrentTime;
			this->myFirstTimeFlag = false;
		}

		if(this->myIsSleepingFlag)
		{
			// Fix FPU exceptions
			__try
			{
				if((MI_Time::ourCurrentTime - this->myFireTime) > this->myReloadTime)
				{
					this->myExitedSleepFlag = true;
					this->myIsSleepingFlag = false;
				}
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				this->myFireTime = MI_Time::ourCurrentTime;
				this->myReloadTime = MI_Time::ourCurrentTime;
			}
		}
	}
};

CHECK_OFFSET(EX_CAI_SupportWeaponManager, myGameWrapper, 0xC);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myValue, 0x10);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myApCost, 0x14);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myTypeName, 0x18);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myReloadTime, 0x118);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myFireTime, 0x11C);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myFiringPosition, 0x120);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myFiringDirection, 0x12C);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myFiringFitness, 0x134);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myCommanderAiData, 0x138);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myIsSleepingFlag, 0x13C);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myExitedSleepFlag, 0x13D);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myFirstTimeFlag, 0x13E);
CHECK_OFFSET(EX_CAI_SupportWeaponManager, myIsBridgeRepairer, 0x13F);