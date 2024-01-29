#include "EX_CAI_Type.h"

u32 EX_CAI_Type::GetWeaponDamageDirect() const
{
	const auto index = static_cast<u32>(std::max(myMaxDamageShooterIndex, 0));

	if (index >= myShooterData.size())
		return 0;

	return myShooterData[index].myWeaponDamageDirect;
}

u32 EX_CAI_Type::GetWeaponDamageBlast() const
{
	const auto index = static_cast<u32>(std::max(myMaxDamageShooterIndex, 0));

	if (index >= myShooterData.size())
		return 0;

	return myShooterData[index].myWeaponDamageBlast;
}

u32 EX_CAI_Type::GetArmorPiercingDirect() const
{
	const auto index = static_cast<u32>(std::max(myMaxDamageShooterIndex, 0));

	if (index >= myShooterData.size())
		return 0;

	return myShooterData[index].myArmorPiercingDirectValue;
}

u32 EX_CAI_Type::GetArmorPiercingBlast() const
{
	const auto index = static_cast<u32>(std::max(myMaxDamageShooterIndex, 0));

	if (index >= myShooterData.size())
		return 0;

	return myShooterData[index].myArmorPiercingBlastValue;
}

float EX_CAI_Type::GetFiringRate() const
{
	const auto index = static_cast<u32>(std::max(myMaxDamageShooterIndex, 0));

	if (index >= myShooterData.size())
		return 0.0f;

	return myShooterData[index].myFiringRate;
}

u32 EX_CAI_Type::GetBulletsPerMag() const
{
	const auto index = static_cast<u32>(std::max(myMaxDamageShooterIndex, 0));

	if (index >= myShooterData.size())
		return 0;

	return myShooterData[index].myMagazineCapacity;
}

float EX_CAI_Type::GetReloadTime() const
{
	const auto index = static_cast<u32>(std::max(myMaxDamageShooterIndex, 0));

	if (index >= myShooterData.size())
		return 0.0f;

	return myShooterData[index].myReloadTime;
}

float EX_CAI_Type::GetAccuracy() const
{
	const auto index = static_cast<u32>(std::max(myMaxDamageShooterIndex, 0));

	if (index >= myShooterData.size())
		return 0.0f;

	return myShooterData[index].myAccuracy;
}