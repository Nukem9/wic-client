#include "MG_Manager.h"

bool MG_Manager::Activate(s32 aGuiId, s32 anActivatingGui)
{
	return ((bool(__stdcall *)(MG_Manager *, s32, s32))0x009BBB50)(this, aGuiId, anActivatingGui);
}