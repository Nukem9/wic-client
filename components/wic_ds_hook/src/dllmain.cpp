#include "stdafx.h"

void Server_PatchAssertions()
{
	auto patchAssert = [](uintptr_t Address)
	{
		// Disable assert by setting 'ignoreAlwaysFlag' to true
		*reinterpret_cast<bool *>(Address) = true;
	};

	patchAssert(0x00959B0A);// CAI_Assert(".\\EX_CAI_Type.cpp", 281, "0", &byte_959B0A);
	patchAssert(0x00960160);// CAI_Assert(".\\ex_cai_decisiontree_node_branching.cpp", 166, "lowest<100000.0f", &byte_960160);
	patchAssert(0x009598F4);// CAI_Assert(".\\EX_AIPlayerContainer.cpp", 2246, "movementSpeed>0 && movementSpeed<900000", &byte_9598F4);
	patchAssert(0x00960023);// CAI_Assert(".\\ex_cai_scoutmap.cpp", 215, "tile>=0", &byte_960023);
	patchAssert(0x00959BAA);// CAI_Assert(".\\ex_cai_militarybrain.cpp", 470, "guard<100", &byte_959BAA);
	patchAssert(0x0096013C);// CAI_Assert(".\\ex_cai_supportweaponmanager.cpp", 926, "length*2<aSupportWeapon->myTargetAreaLength", &byte_96013C);

	patchAssert(0x008F36E8);// MC_Assert(".\\EXG_Projectile.cpp", 413, "aBlastRadius > 0.0f", &byte_8F36E8);
	patchAssert(0x008F39A9);// MC_Assert(".\\EXG_MovementSpline.cpp", 106, "myWaypoints[29].myWaypoint.y >= minHeight", &byte_8F39A9);
	patchAssert(0x008F3988);// MC_Assert(".\\EXG_CopterMover.cpp", 546, "minHeight <= destination.y", &byte_8F3988);
	patchAssert(0x008F3965);// MC_Assert(".\\WICG_MultiAgentShooterTarget.cpp", 147, "myNumTargets", &byte_8F3965);
	patchAssert(0x008EF13C);// MC_Assert("c:\\p4ws\\wic-UPDATE11\\Framework\\MCommon2\\mc_keytree.h", 161, "aKey == current->myKey", &ignoreAlwaysFlag);
	patchAssert(0x008F3516);// MC_Assert("c:\\p4ws\\wic-UPDATE11\\Framework\\MCommon2\\mc_keytree.h", 161, "aKey == current->myKey", &ignoreAlwaysFlag);
	patchAssert(0x008F01ED);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 227, "iFromGridX >= 0 && iFromGridX < mmInfo.myNumPatchesX", &byte_8F01ED);
	patchAssert(0x008F01EC);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 228, "iFromGridZ >= 0 && iFromGridZ < mmInfo.myNumPatchesZ", &byte_8F01EC);
	patchAssert(0x008F01EB);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 229, "iToGridX >= 0 && iToGridX < mmInfo.myNumPatchesX", &byte_8F01EB);
	patchAssert(0x008F01EA);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 230, "iToGridZ >= 0 && iToGridZ < mmInfo.myNumPatchesZ", &byte_8F01EA);
	patchAssert(0x008F01E9);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 285, "ix>= 0 && ix < mmInfo.myNumPatchesX", &byte_8F01E9);
	patchAssert(0x008F01E8);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 286, "ix >= 0 && ix < mmInfo.myNumPatchesZ", &byte_8F01E8);
	patchAssert(0x008F01E7);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 287, "iz >= 0 && iz < mmInfo.myNumPatchesX", &byte_8F01E7);
	patchAssert(0x008F01E6);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 288, "iz>= 0 && iz < mmInfo.myNumPatchesZ", &byte_8F01E6);
	patchAssert(0x008F01E5);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 402, "ix>= 0 && ix < mmInfo.myNumPatchesX", &byte_8F01E5);
	patchAssert(0x008F01E4);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 403, "ix >= 0 && ix < mmInfo.myNumPatchesZ", &byte_8F01E4);
	patchAssert(0x008F01E3);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 404, "iz >= 0 && iz < mmInfo.myNumPatchesX", &byte_8F01E3);
	patchAssert(0x008F01E2);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 405, "iz>= 0 && iz < mmInfo.myNumPatchesZ", &byte_8F01E2);
	patchAssert(0x008F3693);// MC_Assert(".\\EXG_Container.cpp", 1052, "0 && \"No free slot in container\"", &byte_8F3693);
	patchAssert(0x008F6A4F);// MC_Assert(".\\WICO_StatsManager.cpp", 32, "aPlayerNum < EX_MAX_NUM_PLAYERS", &byte_8F6A4F);
	patchAssert(0x0095986E);// MC_Assert(".\\EXCO_CloudType.cpp", 88, "myTimeToLive > 0.0f && \"EXCO_CloudType::EXCO_CloudType(): myTimeToLive was <= 0.0f, this is illegal!\"", &byte_95986E)
	patchAssert(0x0095986C);// MC_Assert(".\\EXCO_CloudType.cpp", 91, "myRadius > 0.0f && \"EXCO_CloudType::EXCO_CloudType(): myRadius was <= 0.0f, this is illegal!\"", &byte_95986C)
}

void WicDS_HookInit(HMODULE hModule)
{
	if (_stricmp(reinterpret_cast<const char *>(0x00753C64), "henrik.davidsson/MSV-BUILD-04 at 10:57:07 on Jun 10 2009.\n") != 0)
	{
		MessageBoxA(nullptr, "Unknown dedicated server version detected. Version 1.0.1.1 is required.", "Error", MB_ICONERROR);
		return;
	}

	//
	// Check for a custom server framerate option on the command line ("wic_ds.exe -fps 12345"). Defaults to 10 FPS.
	//
	int argCount = 0;

	if (auto commandLine = CommandLineToArgvW(GetCommandLineW(), &argCount))
	{
		for (int i = 0; i < (argCount - 1); i++)
		{
			if (!_wcsicmp(commandLine[i], L"-fps"))
			{
				// Server FPS divisor
				// (1000 / FrameTime)	= FPS
				// (1000 / FPS)			= FrameTime
				uint32_t frameTime = 1000 / std::max(_wtoi(commandLine[i + 1]), 10);
				PatchMemory(0x00402B1F, reinterpret_cast<uint8_t *>(&frameTime), sizeof(frameTime));

				// Ignore timer errors when a higher framerate is used
				PatchMemory(0x0041B5ED, { 0xE9, 0xDD, 0x00, 0x00, 0x00 });

				// Disable console title updates every frame
				PatchMemory(0x00405A40, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
			}
		}

		LocalFree(commandLine);
	}

	//
	// Force the dedicated server to look in the "mods" folder relative to the current directory. Vanilla
	// code checks in X:\Users\Public\Documents\WIC\Mods.
	//
	EXCO_Directory::InitializeHook();

	//
	// Drop invalid array accesses (removed assertions) in some AI code
	//
	EX_CAI_Type::InitializeHook();

	Server_PatchAssertions();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	WicDS_HookInit(hModule);
	return TRUE;
}