#include <algorithm>
#include "../mcommon2/MC_Str.h"
#include "EX_CAI_AI.h"

void EX_CAI_AI::hk_SetAP(const int anAmount)
{
	int restrictedAmount = anAmount;

	if (myRoleId == MC_StringToInt("AIROLE_INFANTRY") && restrictedAmount > 10000)
		restrictedAmount = 10000;

	myApAmount = restrictedAmount;
	((void(__thiscall *)(EX_CAI_CommanderAI *, float))0x006A9F90)(myCommanderAI, restrictedAmount);
}

void EX_CAI_AI::hk_SetMaxAP(const int anAmount)
{
	int restrictedAmount = anAmount;

	if (myRoleId == MC_StringToInt("AIROLE_INFANTRY") && restrictedAmount > 10000)
		restrictedAmount = 10000;

	((void(__thiscall *)(EX_CAI_CommanderAI *, float))0x006A9FB0)(myCommanderAI, restrictedAmount);
}

void EX_CAI_AI::hk_SetCurrentMax(const int anAmount)
{
	int restrictedAmount = anAmount;

	if (myRoleId == MC_StringToInt("AIROLE_INFANTRY") && restrictedAmount > 10000)
		restrictedAmount = 10000;

	((void(__thiscall *)(EX_CAI_CommanderAI *, const int))0x006A9FD0)(myCommanderAI, restrictedAmount);
}