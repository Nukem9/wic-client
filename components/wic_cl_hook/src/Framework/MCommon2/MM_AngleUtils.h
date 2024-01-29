#pragma once

class MM_AngleUtils
{
public:
	static void CropAngleDiff(float& aDiff);
	static void CropAngle(float& anAngle);
	static float AngleDiff(const float& aDest, const float& aSrc);
	static float MoveToAngle(float& aSrc, const float& aDest, float aMove);
};