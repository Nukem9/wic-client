#include <EX_3D/EX3D_Console.h>
#include "WICP_DebugView.h"

bool WICP_DebugView::Init()
{
	EX3D_Console::ourInstance->AddFunc("DebugToggle", Console_DebugScriptToggle, this, false, nullptr);
	EX3D_Console::ourInstance->AddFunc("DebugToggleDevMenu", Console_DebugToggleDevMenu, this, false, nullptr);
	EX3D_Console::ourInstance->AddFunc("DebugTestCommand", Console_DebugTestCommand, this, false, nullptr);

	return ((bool(__stdcall *)(WICP_DebugView *))0x008FF040)(this);
}

void WICP_DebugView::Activate(bool anActiveFlag)
{
	myIsActiveFlag = anActiveFlag;
}

void WICP_DebugView::ActivateMenu(bool anActiveFlag)
{
	myIsMenuActiveFlag = anActiveFlag;
}

void WICP_DebugView::SetCycleValue(int aValue)
{
	myFlags[aValue]++;

	if (myFlags[aValue] == myItems[aValue].myNumCycleStates)
		myFlags[aValue] = 0;

	if (myItems[aValue].myType == WICO_DebugView::DVT_CYCLE_BUTTON)
		PropagateToGroup(static_cast<WICO_DebugView::DV_Flag>(aValue), true);
	else
		PropagateToGroup(static_cast<WICO_DebugView::DV_Flag>(aValue), myFlags[aValue] == 1);
}

void WICP_DebugView::SetToggleValue(int aValue)
{
	if (myItems[aValue].myType == WICO_DebugView::DVT_TOGGLE_RADIO_BUTTON)
		myFlags[aValue] = 1;
	else
		myFlags[aValue] = myFlags[aValue] != 1;

	if (myItems[aValue].myAlias != WICO_DebugView::DVF_NUMTYPES)
	{
		int aliasValue = myItems[aValue].myAliasSetToValue;

		if (aliasValue <= -1)
			aliasValue = myFlags[aValue];

		myFlags[myItems[aValue].myAlias] = aliasValue;
	}

	if (myItems[aValue].myType == WICO_DebugView::DVT_TOGGLE_GROUP_BUTTON)
		PropagateToGroup(static_cast<WICO_DebugView::DV_Flag>(aValue), myFlags[aValue] == 1);
	else if (myItems[aValue].myType == WICO_DebugView::DVT_TOGGLE_RADIO_BUTTON)
		PropagateToRadioGroup(static_cast<WICO_DebugView::DV_Flag>(aValue), myFlags[aValue] == 1);
}

void WICP_DebugView::PropagateToGroup(WICO_DebugView::DV_Flag aFlag, const bool aEnableFlag)
{
	((void(__thiscall *)(WICP_DebugView *, WICO_DebugView::DV_Flag, const bool))0x008FEC00)(this, aFlag, aEnableFlag);
}

void WICP_DebugView::PropagateToRadioGroup(WICO_DebugView::DV_Flag aFlag, const bool aEnableFlag)
{
	((void(__thiscall *)(WICP_DebugView *, WICO_DebugView::DV_Flag, const bool))0x008FE9F0)(this, aFlag, aEnableFlag);
}

int WICP_DebugView::GetTypeForString(const char *aString)
{
	static const char *flagNames[] =
	{
		"DVF_EFFECTS",
		"DVF_GAME_GROUP",
		"DVF_GAME_INFO",
		"DVF_GAME_UNITS",
		"DVF_GAME_PROJECTILES",
		"DVF_GAME_CLOUDS",
		"DVF_GAME_BUILDINGS",
		"DVF_GAME_PROPS",
		"DVF_GAME_COMMANDPOINTS",
		"DVF_GAME_DESTRUCTIBLE",
		"DVF_PLAYER_INFO",
		"DVF_RELATIONS",
		"DVF_TRIGGERS",
		"DVF_LOS_CHEAT",
		"DVF_PLAYER_GROUP",
		"DVF_PLAYER_STATS",
		"DVF_PLAYER_UNIT",
		"DVF_PLAYER_UNIT_DAMAGE",
		"DVF_PLAYER_UNIT_VISUAL_REP",
		"DVF_PLAYER_UNIT_PHYSIC_REP",
		"DVF_PLAYER_UNIT_CROUCHING",
		"DVF_PLAYER_BUILDINGS",
		"DVF_PLAYER_BUILDINGS_MORE",
		"DVF_PLAYER_WORLD",
		"DVF_PLAYER_CIVILIANS",
		"DVF_PLAYER_FENCES",
		"DVF_PLAYER_PROPS",
		"DVF_PLAYER_SOUND",
		"DVF_PLAYER_SOUND_USAGE",
		"DVF_PLAYER_FEEDBACK_STRESS",
		"DVF_MAPDATA_GROUP",
		"DVF_MAPDATA_MAP_MODE",
		"DVF_GAME_GROUP_TOGGLE",
		"DVF_PLAYER_GROUP_TOGGLE",
		"DVF_ART_MAIN",
		"DVF_ART_PATHFINDING",
		"DVF_ART_PATHFINDING_NONE",
		"DVF_ART_PATHFINDING_TANKS",
		"DVF_ART_PATHFINDING_INFANTRY",
		"DVF_ART_PATHFINDING_HELICOPTERS",
		"DVF_ART_PATHFINDING_VEHICLES",
		"DVF_ART_MRB_DIRECTORY_PATH",
		"DVF_ART_MRB_DIRECTORY_PATH_HOUSE",
		"DVF_ART_MRB_DIRECTORY_PATH_PROPS",
		"DVF_ART_UNITS",
		"DVF_ART_UNITS_PHYSICAL_REP",
		"DVF_ART_UNITS_VISUAL_REP",
		"DVF_ART_HOUSE",
		"DVF_ART_HOUSE_SLOT_INFO",
		"DVF_ART_HOUSE_SLOT_INFO_POSITIONS",
		"DVF_ART_HOUSE_SLOT_INFO_DOORS",
		"DVF_ART_HOUSE_SLOT_INFO_DIRECTIONS",
		"DVF_ART_HOUSE_BOUNDING_BOX",
		"DVF_ART_PHYS_BOXES",
		"DVF_ART_PHYS_BOXES_HOUSE",
		"DVF_ART_PHYS_BOXES_PROPS",
		"DVF_ART_PHYS_BOXES_UNITS",
		"DVF_NUMTYPES",
	};

	for (size_t i = 0; i < std::size(flagNames); i++)
	{
		if (!strcmp(flagNames[i], aString))
			return i;
	}

	return WICO_DebugView::DVF_NUMTYPES;
}

bool WICP_DebugView::Console_DebugScriptToggle(char *aString, void *aUserData)
{
	char menuType[255];
	auto debugView = static_cast<WICP_DebugView *>(aUserData);

	if (sscanf_s(aString, "%s", menuType, 255) == 1)
	{
		int type = GetTypeForString(menuType);

		if (type == WICO_DebugView::DVF_NUMTYPES)
		{
			strcpy_s(aString, 4096, "Unknown command");
		}
		else
		{
			if (debugView->myItems[type].myType == WICO_DebugView::DVT_CYCLE_BUTTON)
				debugView->SetCycleValue(type);
			else
				debugView->SetToggleValue(type);

			//profile->SetDebugViewFlags(debugView->myFlags);
			debugView->Activate(true);
			debugView->ActivateMenu(false);
		}
	}
	else
	{
		strcpy_s(aString, 4096, "Bad command");
	}

	return true;
}

bool WICP_DebugView::Console_DebugToggleDevMenu(char *aString, void *aUserData)
{
	auto debugView = static_cast<WICP_DebugView *>(aUserData);

	debugView->ActivateMenu(true);
	debugView->Activate(!debugView->myIsActiveFlag);

	return true;
}

#include "../EX_Game/WICG_Root.h"
#include "../EX_Game/EXG_Game.h"
#include "../EX_Game/EXG_PlayerContainer.h"
#include "../EX_Game/EXG_Player.h"
#include "EXP_Player.h"

#include "WICP_Root.h"
#include "../EX_Common/EXCO_IGame.h"

#include <EX_Common/EXCO_ErrorMessage.h>
#include <MGui/MG_Manager.h>
#include <MCommon2/MC_String.h>

#include <chrono>

std::chrono::steady_clock::time_point startTime;

bool __fastcall hk_ReceiveChatPrivate(EXP_Player *aThis, void *_EDX, const EXCO_PlayerInfo *aPlayer, const wchar_t *aMessage)
{
	auto endTime = std::chrono::high_resolution_clock::now();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

	EX3D_Console::ourInstance->StuffText("Received chat command! %ls", aMessage);
	EX3D_Console::ourInstance->StuffText("Latency: %llu", milliseconds.count());

	return 1;
}

bool WICP_DebugView::Console_DebugTestCommand(char *aString, void *aUserData)
{
	auto gameHandler = WICP_Root::ourGameHandler;
	auto ourPlayer = WICP_Root::ourPlayer;

	startTime = std::chrono::high_resolution_clock::now();
	if (gameHandler)
		gameHandler->SendChatPrivate(*ourPlayer, L"<<@@LATENCY@@>>", &ourPlayer->GetInfo());

	EXCO_ErrorMessage::ourInstance->AddError(L"Test error string!", EXCO_ErrorMessage::MASSGATE_NOTIFICATION, nullptr);
	WICP_Root::ourPlayer->myGuiManager.Activate(MC_StringToInt("WiC_GenericErrorDialog"), 0);
	return true;
}