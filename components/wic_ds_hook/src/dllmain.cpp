#include "stdafx.h"

/*
class EXD_DedicatedServer
{
private:
	//class EXCO_MissionInfo myMission;	// this+0x0
	char myMission[0x2CF0];				// this+0x0

	WICR_ClientFactory		*myClientFactory;		 // this+0x2CF0
	WICR_GameController		*myGameController;		 // this+0x2CF4
	EXCO_SlotManager		mySlotManager;		 // this+0x2CF8
	EXCO_MissionSet			myMissions;		 // this+0x2D08
	EXCO_MissionSet			myMapCycle;		 // this+0x2D2C
	const Ice				*myGameText;		 // this+0x2D50
	bool					myReportedStatsFlag;		 // this+0x2D54
	int						myRestartInterval;		 // this+0x2D58
	int						myNumGames;		 // this+0x2D5C
	MC_Str<wchar_t,0>		myGamename;		 // this+0x2D60
	MC_Str<wchar_t,0>		myModName;		 // this+0x2D64
	WICR_GameController::WICR_GameInitData	myIniData;		 // this+0x2D68
	float					myNextDebugTime;		 // this+0x312C
	unsigned int	myNumFrames;		 // this+0x3130
	class WICO_GameSettings	myGameSettings;		 // this+0x3134
	unsigned int[0x78]	myTempBuffer;		 // this+0x3564
	class WICG_VoipServer	myVoipServer;		 // this+0x3744
	float	myGracefulShutdownTime;		 // this+0x3790
	float	myNextWarningTime;		 // this+0x3794
	bool	myHaveReportedShutdownFlag;		 // this+0x3798

public:
	bool Update()
	{
		if(MMG_TrackableServer::GetInstance())
			MMG_TrackableServer::GetInstance()->Update();

		if (this->myGameController->myGame && !this->myClientFactory->ListenForClients())
		{
			MC_Debug::InternalPosTracer tracer(MC_Debug::TRACE_ERROR, ".\\exd_dedicatedserver.cpp", "EXD_DedicatedServer::Update", 601);
			tracer("myClientFactory->ListenForClients() error");

			return false;
		}

		this->PrintStats();

		if (this->myGracefulShutdownTime > 0.0f)
		{
			if (MI_Time::ourCurrentTime > this->myGracefulShutdownTime)
			{
				MC_Debug::InternalPosTracer tracer(MC_Debug::TRACE_DEBUG, ".\\exd_dedicatedserver.cpp", "EXD_DedicatedServer::Update", 612);
				tracer("Initiating automatic shutdown.");
			}

			EXD_DedicatedServer::UpdateShutdownState(v1);
		}


	}

	void PrintStats()
	{
		// EXD_DedicatedServer::PrintStats
		return ((void (__thiscall *)(EXD_DedicatedServer *))0x00405370)(this);
	}
};
*/

class WICO_RoleManager
{
public:
	class PlayerRole
	{
	};
};

class EXG_Game;

bool(__thiscall * EXG_Game__PlayerSetRole)(EXG_Game *thisptr, EXCO_IPlayer *aPlayer, const int aRoleId, bool aIsForcedSwitchedFlag);

bool __fastcall hk_EXG_Game__PlayerSetRole(EXG_Game *thisptr, void *_EDX, EXCO_IPlayer *aPlayer, const int aRoleId, bool aIsForcedSwitchedFlag)
{
	if (aRoleId == MC_StringToInt("ARMY_ROLE"))
		return true;

	return EXG_Game__PlayerSetRole(thisptr, aPlayer, aRoleId, aIsForcedSwitchedFlag);
}

bool (__thiscall * MC_KeyTree_WICO_RoleManager__PlayerRole_int___Add)(void *thisptr, WICO_RoleManager::PlayerRole *const *anInstance, const int *aKey);

bool __fastcall hk_MC_KeyTree_WICO_RoleManager__PlayerRole_int___Add(void *thisptr, void *_EDX, WICO_RoleManager::PlayerRole *const *anInstance, const int *aKey)
{
	if (*aKey == MC_StringToInt("ARMY_ROLE"))
		return true;

	return MC_KeyTree_WICO_RoleManager__PlayerRole_int___Add(thisptr, anInstance, aKey);
}

bool(__thiscall * MC_GrowingArray_WICO_RoleManager__PlayerRole___Add)(MC_GrowingArray<WICO_RoleManager::PlayerRole *> *thisptr, WICO_RoleManager::PlayerRole *const *anItem);

bool __fastcall hk_MC_GrowingArray_WICO_RoleManager__PlayerRole___Add(MC_GrowingArray<WICO_RoleManager::PlayerRole *> *thisptr, void *_EDX, WICO_RoleManager::PlayerRole *const *anItem)
{
	DWORD ptr = (DWORD)*anItem;

	if (*(DWORD *)(ptr + 0x404) == MC_StringToInt("ARMY_ROLE"))
		return true;

	return MC_GrowingArray_WICO_RoleManager__PlayerRole___Add(thisptr, anItem);
}

bool (__thiscall * EXCO_MissionInfo__ValidatePlayerRole)(EXCO_MissionInfo *thisptr, const int aRole);

bool __fastcall hk_EXCO_MissionInfo__ValidatePlayerRole(EXCO_MissionInfo *thisptr, void *_EDX, const int aRole)
{
	if (aRole == MC_StringToInt("ARMY_ROLE"))
		return true;

	return EXCO_MissionInfo__ValidatePlayerRole(thisptr, aRole);
}

void Server_PatchFPUExceptions()
{
	PatchMemory(0x004024AF, (PBYTE)"\x90\x90\x90\x90\x90", 5);// main(): _clearfp();
	PatchMemory(0x004024BB, (PBYTE)"\x90\x90\x90\x90\x90", 5);// main(): _controlfp();

	PatchMemory(0x0041C5B1, (PBYTE)"\x90\x90\x90\x90\x90", 5);// MT_Thread_thread_starter(): _clearfp();
	PatchMemory(0x0041C5BD, (PBYTE)"\x90\x90\x90\x90\x90", 5);// MT_Thread_thread_starter(): _controlfp();
}

void Server_PatchFramerate(uint Framerate)
{
	// Server FPS divisor
	// (1000 / FrameTime)	= FPS
	// (1000 / FPS)			= FrameTime
	uint frameTime = 1000 / max(Framerate, 10);
	PatchMemory(0x00402B1F, (PBYTE)&frameTime, sizeof(uint));

	// Ignore timer errors when a higher framerate is used
	PatchMemory(0x0041B5ED, (PBYTE)"\xE9\xDD\x00\x00\x00", 5);

	// Disable console title updates every frame
	PatchMemory(0x00405A40, (PBYTE)"\x90\x90\x90\x90\x90\x90\x90", 7);
}

void Server_PatchAssertions()
{
	auto PatchAssert = [](DWORD Address)
	{
		// Disable assert by setting 'ignoreAlwaysFlag' to true
		*(bool *)Address = true;
	};

	PatchAssert(0x00959B0A);// CAI_Assert(".\\EX_CAI_Type.cpp", 281, "0", &byte_959B0A);
	PatchAssert(0x00960160);// CAI_Assert(".\\ex_cai_decisiontree_node_branching.cpp", 166, "lowest<100000.0f", &byte_960160);
	PatchAssert(0x009598F4);// CAI_Assert(".\\EX_AIPlayerContainer.cpp", 2246, "movementSpeed>0 && movementSpeed<900000", &byte_9598F4);
	PatchAssert(0x00960023);// CAI_Assert(".\\ex_cai_scoutmap.cpp", 215, "tile>=0", &byte_960023);
	PatchAssert(0x00959BAA);// CAI_Assert(".\\ex_cai_militarybrain.cpp", 470, "guard<100", &byte_959BAA);
	PatchAssert(0x0096013C);// CAI_Assert(".\\ex_cai_supportweaponmanager.cpp", 926, "length*2<aSupportWeapon->myTargetAreaLength", &byte_96013C);

	PatchAssert(0x008F36E8);// MC_Assert(".\\EXG_Projectile.cpp", 413, "aBlastRadius > 0.0f", &byte_8F36E8);
	PatchAssert(0x008F39A9);// MC_Assert(".\\EXG_MovementSpline.cpp", 106, "myWaypoints[29].myWaypoint.y >= minHeight", &byte_8F39A9);
	PatchAssert(0x008F3988);// MC_Assert(".\\EXG_CopterMover.cpp", 546, "minHeight <= destination.y", &byte_8F3988);
	PatchAssert(0x008F3965);// MC_Assert(".\\WICG_MultiAgentShooterTarget.cpp", 147, "myNumTargets", &byte_8F3965);
	PatchAssert(0x008EF13C);// MC_Assert("c:\\p4ws\\wic-UPDATE11\\Framework\\MCommon2\\mc_keytree.h", 161, "aKey == current->myKey", &ignoreAlwaysFlag);
	PatchAssert(0x008F3516);// MC_Assert("c:\\p4ws\\wic-UPDATE11\\Framework\\MCommon2\\mc_keytree.h", 161, "aKey == current->myKey", &ignoreAlwaysFlag);
	PatchAssert(0x008F01ED);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 227, "iFromGridX >= 0 && iFromGridX < mmInfo.myNumPatchesX", &byte_8F01ED);
	PatchAssert(0x008F01EC);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 228, "iFromGridZ >= 0 && iFromGridZ < mmInfo.myNumPatchesZ", &byte_8F01EC);
	PatchAssert(0x008F01EB);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 229, "iToGridX >= 0 && iToGridX < mmInfo.myNumPatchesX", &byte_8F01EB);
	PatchAssert(0x008F01EA);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 230, "iToGridZ >= 0 && iToGridZ < mmInfo.myNumPatchesZ", &byte_8F01EA);
	PatchAssert(0x008F01E9);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 285, "ix>= 0 && ix < mmInfo.myNumPatchesX", &byte_8F01E9);
	PatchAssert(0x008F01E8);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 286, "ix >= 0 && ix < mmInfo.myNumPatchesZ", &byte_8F01E8);
	PatchAssert(0x008F01E7);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 287, "iz >= 0 && iz < mmInfo.myNumPatchesX", &byte_8F01E7);
	PatchAssert(0x008F01E6);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 288, "iz>= 0 && iz < mmInfo.myNumPatchesZ", &byte_8F01E6);
	PatchAssert(0x008F01E5);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 402, "ix>= 0 && ix < mmInfo.myNumPatchesX", &byte_8F01E5);
	PatchAssert(0x008F01E4);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 403, "ix >= 0 && ix < mmInfo.myNumPatchesZ", &byte_8F01E4);
	PatchAssert(0x008F01E3);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 404, "iz >= 0 && iz < mmInfo.myNumPatchesX", &byte_8F01E3);
	PatchAssert(0x008F01E2);// MC_Assert(".\\WICO_HierarchicalHeightMap.cpp", 405, "iz>= 0 && iz < mmInfo.myNumPatchesZ", &byte_8F01E2);
	PatchAssert(0x008F3693);// MC_Assert(".\\EXG_Container.cpp", 1052, "0 && \"No free slot in container\"", &byte_8F3693);
	PatchAssert(0x008F6A4F);// MC_Assert(".\\WICO_StatsManager.cpp", 32, "aPlayerNum < EX_MAX_NUM_PLAYERS", &byte_8F6A4F);
}

struct hostent *PASCAL hk_gethostbyname(const char *name)
{
	if (strstr(name, "massgate.net"))
		name = "liveaccount.massgate.org";

	return gethostbyname(name);
}

void WINAPI hk_GetSystemInfo(LPSYSTEM_INFO lpSystemInfo)
{
	GetSystemInfo(lpSystemInfo);

	// Restrict core count to 16 because of a hardcoded array in MT_Supervisor
	if (lpSystemInfo)
		lpSystemInfo->dwNumberOfProcessors = min(lpSystemInfo->dwNumberOfProcessors, 16);
}

DWORD_PTR WINAPI hk_SetThreadAffinityMask(HANDLE hThread, DWORD_PTR dwThreadAffinityMask)
{
	// Don't change anything, the OS knows better than the game
	return 0xFFFFFFFF;
}

BOOL WicDS_HookInit(HMODULE hModule, DWORD ul_reason_for_call)
{
	MMG_Protocols::MassgateProtocolVersion = 150;

	int argCount = 0;
	LPWSTR *commandLine = CommandLineToArgvW(GetCommandLineW(), &argCount);

	for (int i = 0; i < argCount; i++)
	{
		if (wcsicmp(commandLine[i], L"-fps") != 0)
			continue;

		if ((i + 1) >= argCount)
			continue;

		// -fps 12345
		Server_PatchFramerate(_wtoi(commandLine[i + 1]));
	}

	Server_PatchFPUExceptions();
	Server_PatchAssertions();

	EXCO_Directory::InitializeHook();
	EX_CAI_Type::InitializeHook();

	DWORD d = 0;

	// Redirect DNS lookups
	VirtualProtect((LPVOID)0x0074D3A8, 4, PAGE_EXECUTE_READWRITE, &d);
	*(DWORD *)0x0074D3A8 = (DWORD)&hk_gethostbyname;

	// Hook GetSystemInfo (IAT)
	VirtualProtect((LPVOID)0x0074D0F8, 4, PAGE_EXECUTE_READWRITE, &d);
	*(DWORD *)0x0074D0F8 = (DWORD)&hk_GetSystemInfo;

	// Hook SetThreadAffinityMask (IAT)
	VirtualProtect((LPVOID)0x00BEC1A4, 4, PAGE_EXECUTE_READWRITE, &d);
	*(DWORD *)0x0074D0F4 = (DWORD)&hk_SetThreadAffinityMask;

	// Allow ranked servers to use mods
	PatchMemory(0x004072E9, (PBYTE)"\xEB", 1);

	//*(PBYTE *)&EXCO_MissionInfo__ValidatePlayerRole = Detours::X86::DetourFunction((PBYTE)0x004478F0, (PBYTE)&hk_EXCO_MissionInfo__ValidatePlayerRole);
	//*(PBYTE *)&MC_KeyTree_WICO_RoleManager__PlayerRole_int___Add = Detours::X86::DetourFunction((PBYTE)0x0044C990, (PBYTE)&hk_MC_KeyTree_WICO_RoleManager__PlayerRole_int___Add);
	//*(PBYTE *)&MC_GrowingArray_WICO_RoleManager__PlayerRole___Add = Detours::X86::DetourFunction((PBYTE)0x0044CC20, (PBYTE)&hk_MC_GrowingArray_WICO_RoleManager__PlayerRole___Add);
	//*(PBYTE *)&EXG_Game__PlayerSetRole = Detours::X86::DetourFunction((PBYTE)0x004AEFD0, (PBYTE)&hk_EXG_Game__PlayerSetRole);

	//
	// Patch for WICG_MPyPlayer::cPlayer_ChatMessage where the developers incorrectly
	// used player slot #0 instead of the script player when sending chat messages.
	// Slot 0 would crash if no player was connected. Slot -1 is used for scripts.
	//
	PatchMemory(0x004EEFF1, (PBYTE)"\xFF", 1);

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if(ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	DisableThreadLibraryCalls(hModule);
	return WicDS_HookInit(hModule, ul_reason_for_call);
}