#include <Hooks/Hooks.h>
#include <Hooks/Memory.h>
#include <Windows.h>

void *hk_mc_allocate(u32 aSize, const char *aFileName, int aLine, void *aThread, const void *aContext, bool anArrayFlag)
{
	return malloc(aSize);
}

void hk_mc_free(void *aPointer, bool anArrayFlag)
{
	free(aPointer);
}

void __declspec(naked) hookworkaround()
{
	const static u32 dwJump = 0x0050A93D;

	// this->myCurrentAimingHeading = this->myAimingForHeading;
	__asm
	{
		lea eax, [esi + 0xE4]
		mov ecx, [esi + 0xDC]
		mov [eax], ecx

		xorps xmm0, xmm0
		push eax

		jmp [dwJump]
	}
}

DECLARE_HOOK_INITIALIZER_DS(TestingZone,
{
	if (true)
	{
		Hooks::WriteJump(0x0061B250, &memmove);
		Hooks::WriteJump(0x00618D00, &memcpy);
		Hooks::WriteJump(0x00619070, &memset);

		Hooks::WriteJump(0x00401DD0, &hk_mc_allocate);// MC_Allocate
		Hooks::WriteJump(0x00401F80, &hk_mc_free);// MC_Free
		Memory::Patch(0x0040251B, { 0xEB });// Force locTempMemEnableFlag off
	}

	Hooks::WriteJump(0x0050A92F, &hookworkaround);

	uint32_t frameTime = 33;
	Memory::Patch(0x00402B1F, reinterpret_cast<uint8_t *>(&frameTime), sizeof(frameTime));
	//Memory::Patch(0x00402B33, { 0x90, 0x90, 0x90, 0x90, 0x90 });

	// Patch for WICG_MPyPlayer::cPlayer_ChatMessage where the developers incorrectly used player slot #0 instead
	// of the script player when sending chat messages. Slot 0 will crash if no player is connected. Slot 255 is
	// used for scripts.
	Memory::Patch(0x004EEFF1, { 0xFF });

	Memory::Patch(0x0055E2B1, { 0x90, 0x90 });// Send client LOS updates every frame instead of once per second
	Memory::Patch(0x00553200, { 0xB0, 0x01, 0xC3 });// Send client unit frames every frame instead of once per three frames



	// Disable processing of GAME_SPECIAL_DEBUG_PRINT_OUTPUT commands in EXR_Client::Parse
	Memory::Patch(0x00557C09, { 0x90, 0x90 });

	// Disable access to the windows registry from python scripts
	Memory::Fill(0x0089A4F0, 0x0, 288);

	// Disable setting of floating point exception flags. Denormals and NANs will raise exceptions and crash
	// the server otherwise.
	Memory::Patch(0x004024AF, { 0x90, 0x90, 0x90, 0x90, 0x90 });// main
	Memory::Patch(0x004024BB, { 0x90, 0x90, 0x90, 0x90, 0x90 });// main
	Memory::Patch(0x0041C5B1, { 0x90, 0x90, 0x90, 0x90, 0x90 });// MT_Thread_thread_starter
	Memory::Patch(0x0041C5BD, { 0x90, 0x90, 0x90, 0x90, 0x90 });// MT_Thread_thread_starter

	// Disable windows error reporting (FaultRep) submissions. There's nobody listening on the other end.
	Memory::Patch(0x0041D1D0, { 0xC3 });

	// Disable XML error log output. There's nobody listening here either.
	Memory::Patch(0x00420684, { 0x31, 0xC0, 0x90, 0x90, 0x90 });

	// Disable a bunch of log spam while loading maps
	Memory::Patch(0x0044B2DD, { 0x90, 0x90, 0x90, 0x90, 0x90 });// "Error EXCO_SlotManager::Init(): failed to create slot for file"
	Memory::Patch(0x0045D6D8, { 0x90, 0x90, 0x90, 0x90, 0x90 });// "Error EXCO_Slot::RotatedRep::Init(): no slots in slot file"

	// Disable bogus assertions by setting 'ignoreAlwaysFlag' to true. NOTE: Many of these are for running MWMod.
	auto disableAssert = [](std::uintptr_t Address)
	{
		*reinterpret_cast<bool *>(Address) = true;
	};

	disableAssert(0x008EF13C);// BUG! MC_Assert("c:\\p4ws\\wic-UPDATE11\\Framework\\MCommon2\\mc_keytree.h", 161, "aKey == current->myKey")
	disableAssert(0x008F3516);// BUG! MC_Assert("c:\\p4ws\\wic-UPDATE11\\Framework\\MCommon2\\mc_keytree.h", 161, "aKey == current->myKey")
	disableAssert(0x008F3693);// BUG! MC_Assert(".\\EXG_Container.cpp", 1052, "0 && \"No free slot in container\"")
	disableAssert(0x008F6A4F);// MC_Assert(".\\WICO_StatsManager.cpp", 32, "aPlayerNum < EX_MAX_NUM_PLAYERS")
	disableAssert(0x008F67F1);// MC_Assert(".\\MN_ReadMessage.cpp", 886, "0 && \"Typecheck failed, wrong type\"")
	disableAssert(0x008F67F0);// MC_Assert(".\\MN_ReadMessage.cpp", 869, "0 && \"Typecheck failed, no data\"")

	disableAssert(0x00960023);// BUG! CAI_Assert(".\\ex_cai_scoutmap.cpp", 215, "tile>=0")
	disableAssert(0x00959BAA);// BUG! CAI_Assert(".\\ex_cai_militarybrain.cpp", 470, "guard<100")
	disableAssert(0x00959B0A);// CAI_Assert(".\\EX_CAI_Type.cpp", 281, "0")
	disableAssert(0x00960160);// CAI_Assert(".\\ex_cai_decisiontree_node_branching.cpp", 166, "lowest<100000.0f")
	disableAssert(0x009598F4);// CAI_Assert(".\\EX_AIPlayerContainer.cpp", 2246, "movementSpeed>0 && movementSpeed<900000")
	disableAssert(0x0096013C);// CAI_Assert(".\\ex_cai_supportweaponmanager.cpp", 926, "length*2<aSupportWeapon->myTargetAreaLength")

	disableAssert(0x008F36E8);// MC_Assert(".\\EXG_Projectile.cpp", 413, "aBlastRadius > 0.0f")
	disableAssert(0x008F39A9);// MC_Assert(".\\EXG_MovementSpline.cpp", 106, "myWaypoints[29].myWaypoint.y >= minHeight")
	disableAssert(0x008F3988);// MC_Assert(".\\EXG_CopterMover.cpp", 546, "minHeight <= destination.y")
	disableAssert(0x008F01ED);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 227, "iFromGridX >= 0 && iFromGridX < mmInfo.myNumPatchesX")
	disableAssert(0x008F01EC);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 228, "iFromGridZ >= 0 && iFromGridZ < mmInfo.myNumPatchesZ")
	disableAssert(0x008F01EB);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 229, "iToGridX >= 0 && iToGridX < mmInfo.myNumPatchesX")
	disableAssert(0x008F01EA);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 230, "iToGridZ >= 0 && iToGridZ < mmInfo.myNumPatchesZ")
	disableAssert(0x008F01E9);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 285, "ix>= 0 && ix < mmInfo.myNumPatchesX")
	disableAssert(0x008F01E8);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 286, "ix >= 0 && ix < mmInfo.myNumPatchesZ")
	disableAssert(0x008F01E7);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 287, "iz >= 0 && iz < mmInfo.myNumPatchesX")
	disableAssert(0x008F01E6);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 288, "iz>= 0 && iz < mmInfo.myNumPatchesZ")
	disableAssert(0x008F01E5);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 402, "ix>= 0 && ix < mmInfo.myNumPatchesX")
	disableAssert(0x008F01E4);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 403, "ix >= 0 && ix < mmInfo.myNumPatchesZ")
	disableAssert(0x008F01E3);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 404, "iz >= 0 && iz < mmInfo.myNumPatchesX")
	disableAssert(0x008F01E2);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 405, "iz>= 0 && iz < mmInfo.myNumPatchesZ")
	disableAssert(0x0095986E);// MC_Assert(".\\EXCO_CloudType.cpp", 88, "myTimeToLive > 0.0f && \"EXCO_CloudType::EXCO_CloudType(): myTimeToLive was <= 0.0f, this is illegal!\"")
	disableAssert(0x0095986C);// MC_Assert(".\\EXCO_CloudType.cpp", 91, "myRadius > 0.0f && \"EXCO_CloudType::EXCO_CloudType(): myRadius was <= 0.0f, this is illegal!\"")
});

static bool TestVersion()
{
	__try
	{
		auto testValue = reinterpret_cast<const char *>(0x00753C64);
		auto expectedValue = "henrik.davidsson/MSV-BUILD-04 at 10:57:07 on Jun 10 2009.\n";

		return memcmp(testValue, expectedValue, strlen(expectedValue)) == 0;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	return false;
}

void WICDS_HookInit(HMODULE Module)
{
	if (!TestVersion())
	{
		MessageBoxA(nullptr, "Unknown dedicated server version detected. Version 1.0.1.1 is required.", "Error", MB_ICONERROR);
		ExitProcess(1);
	}

	if (!Hooks::Initialize())
	{
		MessageBoxA(nullptr, "Fatal error during hook initialization", "Error", MB_ICONERROR);
		ExitProcess(1);
	}
}