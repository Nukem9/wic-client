#pragma once

template<typename T>
class MC_Vector3
{
public:
	union
	{
		T myX;
		T myR;
		T x;
		T r;
	};

	union
	{
		T myY;
		T myG;
		T y;
		T g;
	};

	union
	{
		T myZ;
		T myB;
		T z;
		T b;
	};
};

//CHECK_OFFSET(MC_Vector3<float>, myX, 0x0);
//CHECK_OFFSET(MC_Vector3<float>, myY, 0x4);
//CHECK_OFFSET(MC_Vector3<float>, myZ, 0x8);