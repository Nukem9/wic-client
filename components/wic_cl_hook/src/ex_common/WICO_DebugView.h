#pragma once

class WICO_DebugView
{
public:
	enum DV_Type
	{
		DVT_TOGGLE_BUTTON = 0x0,
		DVT_TOGGLE_GROUP_BUTTON = 0x1,
		DVT_CYCLE_BUTTON = 0x2,
		DVT_TOGGLE_RADIO_BUTTON = 0x3,
		DVT_NUMTYPES = 0x4,
	};

	enum DV_Flag
	{
		DVF_EFFECTS = 0x0,
		DVF_GAME_GROUP = 0x1,
		DVF_GAME_INFO = 0x2,
		DVF_GAME_UNITS = 0x3,
		DVF_GAME_PROJECTILES = 0x4,
		DVF_GAME_CLOUDS = 0x5,
		DVF_GAME_BUILDINGS = 0x6,
		DVF_GAME_PROPS = 0x7,
		DVF_GAME_COMMANDPOINTS = 0x8,
		DVF_GAME_DESTRUCTIBLE = 0x9,
		DVF_PLAYER_INFO = 0xA,
		DVF_RELATIONS = 0xB,
		DVF_TRIGGERS = 0xC,
		DVF_LOS_CHEAT = 0xD,
		DVF_PLAYER_GROUP = 0xE,
		DVF_PLAYER_STATS = 0xF,
		DVF_PLAYER_UNIT = 0x10,
		DVF_PLAYER_UNIT_DAMAGE = 0x11,
		DVF_PLAYER_UNIT_VISUAL_REP = 0x12,
		DVF_PLAYER_UNIT_PHYSIC_REP = 0x13,
		DVF_PLAYER_UNIT_CROUCHING = 0x14,
		DVF_PLAYER_BUILDINGS = 0x15,
		DVF_PLAYER_BUILDINGS_MORE = 0x16,
		DVF_PLAYER_WORLD = 0x17,
		DVF_PLAYER_CIVILIANS = 0x18,
		DVF_PLAYER_FENCES = 0x19,
		DVF_PLAYER_PROPS = 0x1A,
		DVF_PLAYER_SOUND = 0x1B,
		DVF_PLAYER_SOUND_USAGE = 0x1C,
		DVF_PLAYER_FEEDBACK_STRESS = 0x1D,
		DVF_MAPDATA_GROUP = 0x1E,
		DVF_MAPDATA_MAP_MODE = 0x1F,
		DVF_GAME_GROUP_TOGGLE = 0x20,
		DVF_PLAYER_GROUP_TOGGLE = 0x21,
		DVF_ART_MAIN = 0x22,
		DVF_ART_PATHFINDING = 0x23,
		DVF_ART_PATHFINDING_NONE = 0x24,
		DVF_ART_PATHFINDING_TANKS = 0x25,
		DVF_ART_PATHFINDING_INFANTRY = 0x26,
		DVF_ART_PATHFINDING_HELICOPTERS = 0x27,
		DVF_ART_PATHFINDING_VEHICLES = 0x28,
		DVF_ART_MRB_DIRECTORY_PATH = 0x29,
		DVF_ART_MRB_DIRECTORY_PATH_HOUSE = 0x2A,
		DVF_ART_MRB_DIRECTORY_PATH_PROPS = 0x2B,
		DVF_ART_UNITS = 0x2C,
		DVF_ART_UNITS_PHYSICAL_REP = 0x2D,
		DVF_ART_UNITS_VISUAL_REP = 0x2E,
		DVF_ART_HOUSE = 0x2F,
		DVF_ART_HOUSE_SLOT_INFO = 0x30,
		DVF_ART_HOUSE_SLOT_INFO_POSITIONS = 0x31,
		DVF_ART_HOUSE_SLOT_INFO_DOORS = 0x32,
		DVF_ART_HOUSE_SLOT_INFO_DIRECTIONS = 0x33,
		DVF_ART_HOUSE_BOUNDING_BOX = 0x34,
		DVF_ART_PHYS_BOXES = 0x35,
		DVF_ART_PHYS_BOXES_HOUSE = 0x36,
		DVF_ART_PHYS_BOXES_PROPS = 0x37,
		DVF_ART_PHYS_BOXES_UNITS = 0x38,
		DVF_NUMTYPES = 0x39,
	};

	class Item
	{
	public:
		WICO_DebugView::DV_Type myType;
		wchar_t myText[32];
		WICO_DebugView::DV_Flag myParent;
		WICO_DebugView::DV_Flag myAlias;
		int myAliasSetToValue;
		int myNumCycleStates;
		int myRadioGroup;
	};
	static_assert(sizeof(Item) == 0x58, "");
};

class WICP_DebugView
{
public:
	bool myIsActiveFlag;
	bool myIsMenuActiveFlag;
	unsigned int myFont;
	int myFlags[WICO_DebugView::DVF_NUMTYPES];
	WICO_DebugView::Item myItems[WICO_DebugView::DVF_NUMTYPES];
	int myKloe_spi;
	int myKloe_mp5;
	float myKloe_mpm;
	float myKloe_mpf;
	int myKloe_spiform2;
	float myKloe_maspitk;
	float myKloe_mamp5tk;
	float myKloe_maptk;
	bool myKloe_refl;
	bool myKloe_t2;

	bool __stdcall Init();
	void SetCycleValue(int aValue);
	void SetToggleValue(int aValue);
	void PropagateToGroup(WICO_DebugView::DV_Flag aFlag, const bool aEnableFlag);
	void PropagateToRadioGroup(WICO_DebugView::DV_Flag aFlag, const bool aEnableFlag);

	static int GetTypeForString(const char *aString);
	static bool Console_DebugScriptToggle(char *aString, void *aUserData);
	static bool Console_DebugViewToggle(char *aString, void *aUserData);
};