#pragma once

#pragma pack(push, 1)
class WICO_CollisionObject_Instance 
{
public:
	WICO_CollisionObject	*myObject;				// this+0x0
	MC_Matrix33				myOrientation;			// this+0x4
	MC_Matrix33				myTransposedOrientation;// this+0x28
	MC_Vector3<float>		myPosition;				// this+0x4C
};
#pragma pack(pop)