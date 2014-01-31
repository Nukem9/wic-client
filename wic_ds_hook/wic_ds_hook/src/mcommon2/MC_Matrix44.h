#pragma once

class MC_Matrix44
{
public:
	union
	{
		float myMatrix[4][4];

		struct
		{
			MC_Vector3<float> myXAxis;
			float myXAxisW;

			MC_Vector3<float> myYAxis;
			float myYAxisW;

			MC_Vector3<float> myZAxis;
			float myZAxisW;

			MC_Vector3<float> myPos;
			float myPosW;
		};

		struct
		{
			MC_Vector4<float> myXAxis4;
			MC_Vector4<float> myYAxis4;
			MC_Vector4<float> myZAxis4;
			MC_Vector4<float> myPos4;
		};
	};
};

CHECK_OFFSET(MC_Matrix44, myXAxis4, 0x0);
CHECK_OFFSET(MC_Matrix44, myYAxis4, 0x10);
CHECK_OFFSET(MC_Matrix44, myZAxis4, 0x20);
CHECK_OFFSET(MC_Matrix44, myPos4, 0x30);