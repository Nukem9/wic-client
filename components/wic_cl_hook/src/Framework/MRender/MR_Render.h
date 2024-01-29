#pragma once

#include <Hooks/Memory.h>

class MC_Image;
class MR_IndexBuffer;
class MR_RenderSettings;
class MR_VertexBuffer;

class MR_Render
{
public:
	static inline DeclareGlobalRef(MR_Render *, ourInstance, SelectOffset(0x00E14FB4, 0x008B56EC));

	virtual void CleanupStandardShaders();
	virtual void CloseDown();
	virtual void UnloadAllVertexBuffers();
	virtual bool ReloadAllVertexBuffers();
	virtual void SetupSurfacePerformanceMode();
	virtual bool GetVertexBuffer(MR_VertexBuffer *, u8 **, u32, u32 *);
	virtual bool GetIndexedVertexBuffer(MR_VertexBuffer *, MR_IndexBuffer *, u8 **, u32, u16 **, u32, u32 *);
	virtual bool Setup(MR_RenderSettings *);
	virtual bool Update(MR_RenderSettings *);
	virtual bool Resize(s32, s32, s32, s32);
	virtual bool DumpScreen(void *, bool);
	virtual bool SaveScreenshot(const char *, bool, bool);
	virtual MC_Image *CreateScreenshotImage(bool);
	virtual bool DumpScreenForVideo(void *);
	virtual bool BeginDraw(bool);
	virtual void EndDraw(bool);
};