#include <cmath>
#include <numbers>
#include "MM_AngleUtils.h"

constexpr float M_PI = std::numbers::pi_v<float>;
constexpr float M_2PI = M_PI * 2.0f;

void MM_AngleUtils::CropAngle(float& anAngle)
{
	// Wrap the angle such that it lies in the range [0, 2pi]
	anAngle -= M_2PI * std::floorf(anAngle / M_2PI);
}

void MM_AngleUtils::CropAngleDiff(float& aDiff)
{
	// Wrap the angle such that it lies in the range [-pi, pi]
	aDiff -= M_2PI * std::floorf((aDiff + M_PI) / M_2PI);
}

float MM_AngleUtils::AngleDiff(const float& aDest, const float& aSrc)
{
	// Calculate the absolute difference between two angles
	return M_PI - std::fabs(std::fmod(std::fabs(aDest - aSrc), M_2PI) - M_PI);
}

float MM_AngleUtils::MoveToAngle(float& aSrc, const float& aDest, float aMove)
{
	//
	// Interpolate angle aDest to angle aSrc with aMove.
	// 
	// The difference is defined as target angle minus starting angle. We want to minimize
	// movement between said angles.
	//
	// If diff is greater than zero, go clockwise.
	// If diff is less than zero, go counterclockwise.
	//
	float diff = aDest - aSrc;

	if (diff == 0.0f)
	{
		// MODIFIED: When diff is zero, we return zero instead of aMove
		return 0.0f;
	}

	CropAngleDiff(diff);

	if (diff >= 0.0f)
		diff = std::min(diff, aMove);
	else
		diff = -std::min(-diff, aMove);

	aSrc += diff;
	CropAngle(aSrc);

	return diff;
}