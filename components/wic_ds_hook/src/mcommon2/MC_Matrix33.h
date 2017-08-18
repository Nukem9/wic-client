#pragma once

class MC_Matrix33 
{
public:
	union
	{
		float myMatrix[3][3];

		struct 
		{
			MC_Vector3<float> myXAxis;
			MC_Vector3<float> myYAxis;
			MC_Vector3<float> myZAxis;
		};
	};
};

CHECK_OFFSET(MC_Matrix33, myXAxis, 0x0);
CHECK_OFFSET(MC_Matrix33, myYAxis, 0xC);
CHECK_OFFSET(MC_Matrix33, myZAxis, 0x18);