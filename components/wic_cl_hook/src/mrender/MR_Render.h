#pragma once

typedef void MR_RenderSettings;
typedef void MC_Image;
typedef void MR_VertexBuffer;
typedef void MR_Texture;
typedef void MR_Surface;
typedef void MR_SurfacePass;
typedef void MR_SurfaceInstance;
typedef void MR_MemoryTextureData;
typedef void MR_IndexBuffer;
typedef void MR_RenderState;

enum MR_DrawTypes
{
	MR_DRAWTYPES_TRIANGLELIST = 0x0,
	MR_DRAWTYPES_TRIANGLESTRIP = 0x1,
	MR_DRAWTYPES_TRIANGLEFAN = 0x2,
	MR_DRAWTYPES_LINELIST = 0x3,
	MR_DRAWTYPES_LINESTRIP = 0x4,
	MR_DRAWTYPES_QUADLIST = 0x5,
};

enum MR_ColorWriteMask
{
	MR_COLORWRITE_NONE = 0x0,
	MR_COLORWRITE_RED = 0x1,
	MR_COLORWRITE_GREEN = 0x2,
	MR_COLORWRITE_BLUE = 0x4,
	MR_COLORWRITE_ALPHA = 0x8,
	MR_COLORWRITE_RGB = 0x7,
	MR_COLORWRITE_RGBA = 0xF,
};

class MR_Render
{
public:
	CLASSPTR(MR_Render *, 0x00E14FB4, ourInstance);

	virtual void *__vecDelDtor(uint);

	virtual void CleanupStandardShaders	();
	virtual void CloseDown				();

	virtual void UnloadAllVertexBuffers			();
	virtual bool ReloadAllVertexBuffers			();
	virtual void SetupSurfacePerformanceMode	();
	virtual bool GetVertexBuffer				(MR_VertexBuffer *aVertexBuffer, char **aVertexList, uint aNumVertices, uint *aBaseIndex);
	virtual bool GetIndexedVertexBuffer			(MR_VertexBuffer *aVertexBuffer, MR_IndexBuffer *anIndexBuffer, char **aVertexList, uint aNumVertices, ushort **aIndexList, uint aNumIndices, uint *aBaseIndex);

	virtual bool Setup	(MR_RenderSettings *aSettings);
	virtual bool Update	(MR_RenderSettings *aSettings);
	virtual bool Resize	(int aXPos, int aYPos, int aWidth, int aHeight);
	
	virtual bool		DumpScreen				(voidptr_t aBuffer, bool aGrabCurrentScreenFlag);
	virtual bool		SaveScreenshot			(const char *aFileName, bool aCompressedFlag, bool aGrabCurrentScreenFlag);
	virtual MC_Image	*CreateScreenshotImage	(bool aGrabCurrentScreenFlag);
	
	virtual void Virtual0();// Unknown

	virtual bool BeginDraw	(bool aBeginSceneFlag);
	virtual void EndDraw	(bool aEndSceneFlag);
	virtual bool BeginScene	();
	virtual void EndScene	();
	
	virtual void ClearBuffers		(MC_Vector4<int> *, int, uint, float, char);
	virtual bool ClearColorBuffer	(MC_Vector4<int> *, uint, MR_ColorWriteMask);
	
	virtual bool PrepareTexture		(MR_Texture *, uint **, uint, int);
	virtual void ReleaseTexture		(MR_Texture *);
	virtual bool LoadSpecialTexture	(MR_Texture *, const char *);
	
	virtual bool PrepareSurface		(MR_Surface *);
	virtual void ReleaseSurface		(MR_Surface *);
	virtual bool PrepareSurfacePass	(MR_Surface *, MR_SurfacePass *);
	virtual void ReleaseSurfacePass	(MR_SurfacePass *);
	virtual void SelectSurface		(MR_SurfaceInstance *, uint, float, int);
	
	virtual void SelectTexture			(MR_Texture *, uint);
	virtual bool LockTexture			(MR_Texture *, void **, uint *, uint);
	virtual void UnlockTexture			(MR_Texture *, uint);
	virtual void UpdateTextureContents	(MR_Texture *, MR_MemoryTextureData *);
	
	virtual void SetState			(MR_RenderState *aState, int aStateFlags);
	virtual void Set2DState			(bool aTextureFlag);
	virtual bool SetRenderTarget	(MR_Texture *aRenderTarget, bool aSetDepthBufferAlsoFlag, bool aForceResetFlag);
	
	virtual MR_IndexBuffer	*CreateIndexBuffer	(uint aNumIndices, bool aStaticBufferFlag, bool (__cdecl *aFunc)(voidptr_t), voidptr_t aRestoreId);
	virtual void			ReleaseIndexBuffer	(MR_IndexBuffer *aIndexBuffer);
	virtual bool			LockIndexBuffer		(MR_IndexBuffer *aIndexBuffer, ushort **aIndexList, int aStartIndex, int aNumIndices);
	virtual void			UnlockIndexBuffer	(MR_IndexBuffer *aIndexBuffer);
	
	virtual MR_VertexBuffer *CreateVertexBuffer				(uint aVertexSize, uint aNumVertices, bool aStaticBufferFlag, bool (__cdecl *aFunc)(voidptr_t), voidptr_t aRestoreId);
	virtual void			ReleaseVertexBuffer				(MR_VertexBuffer *aVertexBuffer);
	virtual bool			LockVertexBuffer				(MR_VertexBuffer *aVertexBuffer, char **aVertexList, int aStartVertex, int aNumVertices, bool aNoOverwriteFlag);
	virtual void			UnlockVertexBuffer				(MR_VertexBuffer *aVertexBuffer);
	virtual void			DrawVertexBuffer				(MR_VertexBuffer **aVertexBufferList, MR_DrawTypes aType, uint aNumVerts);
	virtual void			DrawIndexedVertexBuffer			(MR_VertexBuffer **aVertexBufferList, MR_IndexBuffer *nIndexBuffer, MR_DrawTypes aType, uint aNumVerts, uint aNumTriangles, uint aNumStatsTransformVerts);
	virtual void			DrawIndexedVertexBufferOffset	(MR_VertexBuffer **aVertexBufferList, MR_IndexBuffer *anIndexBuffer, MR_DrawTypes aType, uint aStartVertex, uint aNumVerts, uint aStartIndex, uint aNumTriangles, uint aNumStatsTransformVerts);
	
	virtual bool BeginIndexedVertices	(MR_DrawTypes aType, const int aNumVertices, const int aVertexSize, char **aVertices, const int aNumIndices, ushort **aIndices);
	virtual void EndIndexedVertices		();
	
	virtual void BeginVertices	(MR_DrawTypes aType, const int aNumVertices, const int aVertexSize, char **aVertices);
	virtual void EndVertices	();
	
	virtual bool CanAllocateIndexedVertices	(const uint, const uint);
	virtual bool SetIndexBufferSize			(uint aNumIndices);
	virtual bool SetVertexBufferSize		(uint aBufferLengthInBytes);

	virtual bool OptimizeMesh		(voidptr_t aVertexList, uint aNumVertices, uint aVertexSize, ushort *anIndexList, uint aNumIndices);
	virtual bool OptimizeIndexList	(ushort *aIndexList, uint aNumIndices, uint aNumVertices);
	
	virtual void Perf_SetMarker		(uint aColor, const wchar_t *aName);
	virtual void Perf_BeginEvent	(uint aColor, const wchar_t *aName);
	virtual void Perf_EndEvent		();
	
	virtual int		CreateOcclusionQuery	();
	virtual void	DestroyOcclusionQuery	(int aHandle);
	virtual void	BeginOcclusionQuery		(int aHandle);
	virtual void	EndOcclusionQuery		(int aHandle);
	virtual int		GetOcclusionReslut		(int aHandle);
	
	virtual bool Wireframe		(bool aEnableFlag);
	virtual void AntialiasLines	(bool aEnableFlag);
	
	virtual void SetScissor					(bool aEnableFlag, int aX, int aY, int aWidth, int aHeight);
	virtual void SetBrightnessContrastGamma	(MC_Vector3<float> aBrightnessRGB, float aContrastValue, float aGammaValue);
	virtual void SetViewport				(uint aXOffset, uint aYOffset, uint aWidth, uint aHeight, float aMinZ, float aMaxZ);
	virtual void SetAlphaRef				(uint aValue);
	virtual void SetPSConstant				(int aIndex, MC_Vector4<float> *aValues);
	virtual void SetPSConstants				(int aIndex, const float *aValues, int aNumConstants);
	
	virtual void EnableViewportTransformation(bool aEnableFlag);

	virtual bool BackbufferToTexture	(MR_Texture *aDestinationTexture, MC_Vector2<int> *aSrcRect, MC_Vector2<int> *aDestRect);
	virtual bool TextureToBackbuffer	(MR_Texture *aSourceTexture, MC_Vector2<int> *aSrcRect, MC_Vector2<int> *aDestRect);
	virtual bool StretchRect			(MR_Texture *aSourceTexture, MR_Texture *aDestinationTexture, MC_Vector2<int> *aSrcRect, MC_Vector2<int> *aDestRect);

	virtual void SetLoadingState(bool aLoadFlag);

	virtual uint GetD3DVersion		();
	virtual uint GetD3DSDKVersion	();

	virtual bool DumpRenderTargetToFile	(MR_Texture *aRenderTarget, const char *aFilename, bool aSaveAlphaFlag, uint aExtraFlags);
	virtual bool MapTextureData			(MR_Texture *aTexture, const voidptr_t *aDataPointer, uint *aRowPitch);
	virtual void UnmapTextureData		(MR_Texture *aTexture);
	virtual bool CopyResource			(MR_Texture *aDestination, MR_Texture *aSource);

	virtual void AcquireThreadOwnership();
	virtual void ReleaseThreadOwnership();

	virtual bool	SupportExtraScreen	(int aWidth, int aHeight);
	virtual int		CreateExtraScreen	(int aWidth, int aHeight);
	virtual void	ReleaseScreen		(int aScreen);
	virtual bool	SwitchScreen		(int aScreen);

	virtual bool SupportExtraDepthTarget			();
	virtual bool SetExtraDepthTarget				(MR_Texture *aTarget);
	virtual bool EnableExtraDepthTargetRendering	(bool aEnableFlag);
	virtual bool RenderToNonAABuffer				(bool aEnable, bool aNeedCopy);
	virtual void ClearAllInternalRenderTargets		();

	virtual void UpdateWindowPosition	();
	virtual void AltTabApp				(bool aIntoApp);

	virtual void PowerSleep();
};