#pragma once

class EXCO_DamageModel
{
public:
	virtual unsigned int CalculateDamage(const unsigned int, const float, MC_Vector3<float> *, MC_Vector3<float> *, const int *, EXCO_Direction::Direction, bool, bool, bool, EXCO_Difficulty::Level);
	virtual unsigned int CalculateAverageDamage(const unsigned int, const int *, EXCO_Direction::Direction);

	virtual float GetAccuracyPenalty(const float);
	virtual float GetSlopeBonus(MC_Vector3<float> *, MC_Vector3<float> *);
	virtual float GetBuildingDamageMultiplier();
	virtual float GetForestDamageMultiplier();
};

CHECK_SIZE(EXCO_DamageModel, 0x4);