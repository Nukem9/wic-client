#pragma once

template<typename T>
class MC_Vector2
{
public:
	union
	{
		T myX;
		T myU;
		T myWidth;
		T x;
	};

	union
	{
		T myZ;
		T myY;
		T myV;
		T myHeight;
		T y;
		T z;
	};
};

//CHECK_OFFSET(MC_Vector2<float>, myX, 0x0);
//CHECK_OFFSET(MC_Vector2<float>, myY, 0x4);