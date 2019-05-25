#pragma once

template<typename T>
class MC_Vector4
{
public:
	union
	{
		T myX;
		T myR;
		T myLeft;
		T x;
		T r;
	};

	union
	{
		T myY;
		T myG;
		T myTop;
		T y;
		T g;
	};

	union
	{
		T myZ;
		T myB;
		T myRight;
		T z;
		T b;
	};

	union
	{
		T myW;
		T myA;
		T myBottom;
		T w;
		T a;
	};
};

//CHECK_OFFSET(MC_Vector4<float>, myX, 0x0);
//CHECK_OFFSET(MC_Vector4<float>, myY, 0x4);
//CHECK_OFFSET(MC_Vector4<float>, myZ, 0x8);
//CHECK_OFFSET(MC_Vector4<float>, myW, 0xC);