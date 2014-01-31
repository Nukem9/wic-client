#pragma once

class MR_RenderD3D10 : public MR_Render
{
public:
	static void InitializeHook();

	static void ForceFlushAllStates();
	
private:
};