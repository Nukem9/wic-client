#pragma once

#pragma pack(push, 1)
class WICO_DebugRenderer 
{
public:
	virtual void DrawCircle(MC_Vector3<float>, const float, const float, const unsigned int);
	virtual void DrawLine(MC_Vector3<float> *, const float, const unsigned int, const unsigned int, bool);
	virtual void DrawLine(MC_Vector3<float> *, const float, const unsigned int);
	virtual void DrawTextA(MC_Vector3<float>, const char *);
	virtual void DrawBitmap(const unsigned int *, const unsigned int, const unsigned int, MC_Vector4<float> *);
	virtual void PrintDebugText(const char *);
	virtual void DrawSphere(MC_Vector3<float>, float, const unsigned int, bool);
	virtual void DrawPhysBox(MC_Matrix44 *, MC_Vector3<float> *, const unsigned int, bool);
};
#pragma pack(pop)