#pragma once

typedef void MR_VertexShaderData;
typedef void MR_VertexShaderSetupData;
typedef void MR_CompiledVertexShader;
typedef void MR_Mesh;

class MR_VertexShader
{
public:
	CLASSPTR(MR_VertexShader *, 0x00E1EE48, ourCurrentShader);

	virtual void *__vecDelDtor(uint32_t);

	virtual void Unload();
	virtual bool Reload();

	virtual MR_CompiledVertexShader *CreateShader	(MR_VertexShaderSetupData *aSetup);
	virtual void					DeleteShader	(MR_CompiledVertexShader *aCompiledShader);

	virtual void Select		(MR_CompiledVertexShader *aCompiledShader);
	virtual bool SetData	(MR_VertexShaderData *aDataBlock);

	virtual bool AllocMeshBuffers	(MR_Mesh *aMesh);
	virtual void DrawMeshList		(MR_Mesh *aMesh, float aLodDistance, float anAnimationTime);

	virtual void RemapVSOutputs(uint32_t *aVSOutputRemaps, MR_VertexShaderSetupData *aVSSetupData);

	virtual int GetMaxNumBonesPerBoneGroup(MR_Mesh *aMesh);

	virtual void GetXYZ(MR_Mesh *aMesh, uint32_t aVertIndex, MC_Vector3<float> *aResultXYZ);

	char			*myName;			// this+0x4
	char			*myAlternativeName;	// this+0x8
	MR_VertexShader *myPrevShader;		// this+0xC
	MR_VertexShader *myNextShader;		// this+0x10
	volatile long	myRefCount;			// this+0x14
	uint32_t		myVertexShaderId;	// this+0x18
};

CHECK_SIZE(MR_VertexShader, 0x1C);