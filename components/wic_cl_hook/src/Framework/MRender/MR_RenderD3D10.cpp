#include "../stdafx.h"

void MR_RenderD3D10::InitializeHook()
{
	uintptr_t virtualTable = 0x00D1E064;

	Detours::X86::DetourFunctionT(0x00A321D0, &ForceFlushAllStates);

	//Detours::X86::DetourClassVTable(virtualTable, &hk_DrawVertexBuffer, 44);
	//Detours::X86::DetourClassVTable(virtualTable, &hk_BeginVertices, 49);
	//Detours::X86::DetourClassVTable(virtualTable, &hk_EndVertices, 50);
}

void MR_RenderD3D10::ForceFlushAllStates()
{
	__asm
	{
		// Random crash on this call, even with an unmodified game
		//mov		eax, dword ptr [00E1F00Ch]	; MR_RenderD3D10::ourD3DDev
		//mov		ecx, [eax]
		//mov		edx, [ecx+114h]
		//push	eax
		//call	edx							; [D3D10Dev] ClearState
	}

	MR_Render::ourInstance->SetRenderTarget(nullptr, true, true);

	// [D3D10CurState] Reset()
	((void (__cdecl *)())0x00A31E20)();

	MR_VertexShader::ourCurrentShader = nullptr;
}