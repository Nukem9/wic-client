#include <Hooks/Hooks.h>
#include "EX3D_Render.h"

void hk_00B092B6();

//
// Fix for a crash when reading past the end of an array in EX3D_Render::QueryModes. If DirectX 10 mode
// is enabled, the API may report an adapter without an output. Game code doesn't account for this.
//
DECLARE_HOOK_INITIALIZER_MP(EX3D_Render,
{
	Hooks::WriteJump(0x00B092B6, &hk_00B092B6);
});

void __declspec(naked) hk_00B092B6()
{
	const static u32 dwDeviceWasNotMatched = 0x00B092BB;
	const static u32 dwDeviceWasAMatch = 0x00B0930F;

	s32 targetDeviceId;
	MR_EnumerationDevice *device;

	__asm
	{
		pushad
		mov ebp, esp
		sub esp, __LOCAL_SIZE
		mov targetDeviceId, edx
		mov device, esi
	}

	if (device->myDeviceId == targetDeviceId)
	{
		if (!device->myOutputs.empty())
		{
			// This device had at least one output. It's a valid match.
			__asm jmp validDeviceFound
		}
	}

	__asm
	{
		// Fallthrough case. Continue the loop.
		mov esp, ebp
		popad
		jmp [dwDeviceWasNotMatched]

	validDeviceFound:
		mov esp, ebp
		popad
		jmp [dwDeviceWasAMatch]
	}
}