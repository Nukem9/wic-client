#pragma once

#include <EX_Common/WICO_DebugView.h>

class WICP_DebugView
{
private:
	bool myIsActiveFlag;
	bool myIsMenuActiveFlag;
	u32 myFont;
	s32 myFlags[WICO_DebugView::DVF_NUMTYPES];
	WICO_DebugView::Item myItems[WICO_DebugView::DVF_NUMTYPES];
	s32 myKloe_spi;
	s32 myKloe_mp5;
	float myKloe_mpm;
	float myKloe_mpf;
	s32 myKloe_spiform2;
	float myKloe_maspitk;
	float myKloe_mamp5tk;
	float myKloe_maptk;
	bool myKloe_refl;
	bool myKloe_t2;

public:
	bool __stdcall Init();
	void Activate(bool anActiveFlag);
	void ActivateMenu(bool anActiveFlag);

private:
	void SetCycleValue(int aValue);
	void SetToggleValue(int aValue);
	void PropagateToGroup(WICO_DebugView::DV_Flag aFlag, const bool aEnableFlag);
	void PropagateToRadioGroup(WICO_DebugView::DV_Flag aFlag, const bool aEnableFlag);

	static int GetTypeForString(const char *aString);
	static bool Console_DebugScriptToggle(char *aString, void *aUserData);
	static bool Console_DebugToggleDevMenu(char *aString, void *aUserData);
	static bool Console_DebugTestCommand(char *aString, void *aUserData);
};