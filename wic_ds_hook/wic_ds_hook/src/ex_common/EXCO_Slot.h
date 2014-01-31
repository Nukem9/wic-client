#pragma once

#pragma pack(push, 1)
class EXCO_Slot 
{
	class RotatedRep 
	{
		class Quadrant 
		{
			uint				myCount;
			MC_Vector3<float>	*myPositions;
		};

		Quadrant			myQuadrants[4];
		uint				myCapacity;
		MC_Vector3<float>	myBounds;
		float				myMarkerHeight;
	};

	RotatedRep	myReps[4];
	const char	*myFileName;
};
#pragma pack(pop)