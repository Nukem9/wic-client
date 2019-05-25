#include <algorithm>
#include "../mcommon2/MC_Str.h"
#include "EXCO_IPlayer.h"

void EXCO_IPlayer::EnforceAPLimits()
{
	// Required for the infantry role due to lots of units crashing the server
	if (true || GetInfo()->GetRole() == MC_StringToInt("ARMY_ROLE"))
	{
		myAP = std::min(myAP, 10000u);
		myCurrentMaxAP = std::min(myCurrentMaxAP, 10000);
	}
}

void EXCO_IPlayer::hk_UpdateAPPenalty(unsigned int aPenaltyDelta)
{
	EXCO_IPlayer *v2; // esi
	int v3; // edx
	unsigned int v4; // ecx
	double v5; // st7
	double v6; // st7
	float v7; // xmm0_4
	double v8; // st6
	float v9; // ST1C_4
	unsigned int v10; // ebx
	unsigned int v11; // eax
	signed int v15; // eax
	int v16; // ecx
	float v17; // [esp+4h] [ebp-Ch]

	v2 = this;
	v3 = this->myRegrowthAP;
	v4 = this->myAP;
	v5 = (double)(v4 + v3);
	v17 = v5;

	if (v5 <= 0.0)
	{
		v7 = 1.0;
		v6 = 0.0;
	}
	else
	{
		v6 = (double)v4 * (float)(1.0 / v17);
		v7 = (float)v3 * (float)(1.0 / v17);
	}

	v8 = (double)aPenaltyDelta;
	v9 = v8;
	v10 = (signed int)(float)(v9 * v7);
	v11 = (signed __int64)(v6 * v8);
	if (v11 > v4)
	{
		v10 += v4 - v11;
		v11 = v4;
	}
	if ((signed int)(aPenaltyDelta - v10 - v11) > 0)
	{
		if (aPenaltyDelta - v10 >= v4)
			v10 = aPenaltyDelta - v11;
		else
			v11 = aPenaltyDelta - v10;
	}

	const WICO_Team::AllowedTeam team = myTeam.GetAllowedTeamForTeam(GetInfo()->GetTeam());

	v2->myAP = v4 - v11;
	v2->myRegrowthAP = v3 - v10;
	v15 = myMission.myGameSettings->GetMaxAp(team);
	v16 = v2->myRegrowthAP + v2->myAP;
	if (v16 >= v15)
		v2->myCurrentMaxAP = v15;
	else
		v2->myCurrentMaxAP = v16;

	EnforceAPLimits();
}

bool EXCO_IPlayer::hk_ChangeAP(const int aDelta)
{
	const WICO_Team::AllowedTeam team = myTeam.GetAllowedTeamForTeam(GetInfo()->GetTeam());

	myAP += aDelta;
	myCurrentMaxAP = std::min(myAP + myRegrowthAP, myMission.myGameSettings->GetMaxAp(team));

	EnforceAPLimits();
	return true;
}

bool EXCO_IPlayer::hk_SetAP(const int aStartVal, const int aRegrowthVal, const float aBattleHonor)
{
	const WICO_Team::AllowedTeam team = myTeam.GetAllowedTeamForTeam(GetInfo()->GetTeam());

	myAP = aStartVal;
	myRegrowthAP = aRegrowthVal;

	if (aStartVal || aRegrowthVal > 0)
		hk_UpdateAPPenalty(myAPPenalty);

	myCurrentMaxAP = std::min(myAP + myRegrowthAP, myMission.myGameSettings->GetMaxAp(team));

	if (aBattleHonor >= 0.0)
		myHonors = aBattleHonor;

	EnforceAPLimits();
	return true;
}

bool EXCO_IPlayer::hk_AddAPRegrowth(const int anAPAmount)
{
	const WICO_Team::AllowedTeam team = myTeam.GetAllowedTeamForTeam(GetInfo()->GetTeam());

	myRegrowthAP += anAPAmount;
	myCurrentMaxAP = std::min(myAP + myRegrowthAP, myMission.myGameSettings->GetMaxAp(team));

	EnforceAPLimits();
	return true;
}

int EXCO_IPlayer::hk_GetAPInUse()
{
	const WICO_Team::AllowedTeam team = myTeam.GetAllowedTeamForTeam(GetInfo()->GetTeam());
	unsigned int maxAp = myMission.myGameSettings->GetMaxAp(team);

	if (GetInfo()->GetRole() == MC_StringToInt("ARMY_ROLE"))
		maxAp = 10000;

	return maxAp - myRegrowthAP - myAP;
}