#pragma once

#include <MCommon2/MC_GrowingArray.h>
#include <MCommon2/MC_Vector.h>
#include <MCommon2/MC_String.h>

class EXP_Unit;

class WICP_MegaMap
{
public:
	enum FilterType : s32
	{
		FILTER_SHOW_DEST_ARROWS = 0,
		FILTER_SHOW_GRID = 1,
		FILTER_NUM_GRID_ELEMENTS = 2,
		FILTER_SHOW_ENEMIES = 3,
		FILTER_SHOW_ALLIES = 4,
		FILTER_SHOW_PLAYER_NAMES = 5,
		FILTER_SHOW_DZ = 6,
		FILTER_SHOW_ALLY_DZ = 7,
		FILTER_SHOW_DZ_INFO = 8,
		FILTER_SHOW_TAS = 9,
		FILTER_SHOW_TA_INFO = 10,
		FILTER_SHOW_CPS = 11,
		FILTER_SHOW_CP_NAMES = 12,
		FILTER_SHOW_REQUESTS = 13,
		FILTER_SHOW_REQUEST_NAMES = 14,
		MAX_NUM_FILTERS = 15,
	};

	class SubUnitGroup
	{
	public:
		MC_Vector4f myRenderingRect;
		EXP_Unit *myUnits[64];
		u32 myNumUnits;
	};
	static_assert_size(SubUnitGroup, 0x114);

	union FilterEntry
	{
		s32 myInt;
	};
	static_assert_size(FilterEntry, 0x4);

	bool myIsEnabledFlag;
	bool myForceMegamap;
	MC_GrowingArray<SubUnitGroup> mySubUnitGroups[17];
	float myLastClickTime;
	MC_Vector2f myLastClickPosition;
	float myUnitIconScale;
	float myDZIconScale;
	float myArrowIconScale;
	const char *myArrowIcon;
	const char *myArrowHeadIcon;
	u32 myDZTextColor;
	u32 myRequestTextColor;
	FilterEntry myFilters[MAX_NUM_FILTERS];
	u32 myTypingLettersCount;
	MC_LocString myTypingLettersText;
	float myTargetIndicatorLetterDelayTime;
	float myLastFrameTime;
	float myNextTargetIndicatorLetterBuiltupTime;
	bool myResetTextFlag;
	float myFlashStartTime;
	bool myIsFlashingFlag;
	bool myWasFlashOnFlag;
	bool myIsSettingDZFlag;
	bool myWasInMegaMapWhenStartingDZPlacingFlag;
	bool myDeployAtDZPlacedFlag;
	float myMegaMapOffset;
	const wchar_t *myPlayerDropZoneText;
	const wchar_t *myPlayerDropTimeText;

	void EndDZPlacing(bool aFlyToDZFlag, bool aIsCancelledFlag);
};
static_assert_size(WICP_MegaMap, 0x1A8);