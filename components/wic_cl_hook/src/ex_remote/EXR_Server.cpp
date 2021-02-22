#include "../stdafx.h"

void EXR_Server::InitializeHook()
{
	Detours::X86::DetourFunctionClass(reinterpret_cast<uint8_t *>(0x0077D481), &ParseUnitFrameShooterChange_0077D481);
}

void __declspec(naked) EXR_Server::ParseUnitFrameShooterChange_0077D481()
{
	static const char *assertString = "shooterIndex < WIC_MAX_NUM_SHOOTERS (8)";
	static const char *assertFile = __FILE__;
	static const int assertLine = __LINE__;

	__asm
	{
		// EDI is shooterIndex which indexes myShooterData[8]
		cmp edi, 8
		jb validArrayIndex

		push 0
		push assertString
		push assertLine
		push assertFile
		call MC_Assert
		add esp, 0x10
		int 3

		validArrayIndex:
		lea edx, dword ptr ds:[edi + edi * 2]
		lea ebp, dword ptr ds:[ebx + edx * 4]
		push 0x0077D487
		retn
	}
}