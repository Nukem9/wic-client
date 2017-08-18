#pragma once

#pragma pack(push, 1)
class WICO_CollisionObject 
{
public:
	virtual void SetOrientation(MC_Matrix33 *);
	virtual MC_Matrix33 *GetOrientation();

	virtual float GetRadius();

	virtual MC_Vector3<float> *GetPosition();
	virtual void SetPosition(MC_Vector3<float> *);

	virtual MC_Vector3<float> *GetCenterPosition(MC_Vector3<float> *result);
	virtual MC_Vector3<float> *GetCenterOffset(MC_Vector3<float> *result);

	virtual bool Intersect(MC_Vector3<float> *, MC_Vector3<float> *, const float, MC_Vector3<float> *, MC_Vector3<float> *, unsigned int *);
	virtual bool Intersect(MC_Vector3<float> *, MC_Vector3<float> *, MC_Vector3<float> *, MC_Vector3<float> *, unsigned int *);
	virtual bool Intersect(MC_Vector3<float> *, MC_Vector3<float> *, const float, MC_Vector3<float> *, MC_Vector3<float> *);
	virtual bool Intersect(MC_Vector3<float> *, MC_Vector3<float> *, MC_Vector3<float> *, MC_Vector3<float> *);
	virtual bool Intersect(MC_Vector3<float> *, MC_Vector3<float> *, const float);
	virtual bool Intersect(MC_Vector3<float> *, MC_Vector3<float> *);

	virtual const unsigned int GetMaterialID();

	virtual void Draw(WICO_DebugRenderer *, MC_Vector3<float> *, MC_Matrix33 *);

	virtual void *__vecDelDtor(unsigned int);

	MC_Matrix33			myOrientation;	// this+0x4
	MC_Vector3<float>	myPosition;		// this+0x28
	float				myRadius;		// this+0x34
	MC_Vector3<float>	myExtents;		// this+0x38
	const unsigned int	myMaterialID;	// this+0x44
	int					myRefs;			// this+0x48
};
#pragma pack(pop)