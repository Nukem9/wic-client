#include "WICR_GameController.h"

bool WICR_GameController::Update()
{
#if WIC_DS
	return ((bool(__thiscall *)(WICR_GameController *))0x005522C0)(this);
#else
	MC_ASSERTMSG(false, "Unreachable client code path");
	__debugbreak();
#endif
}

bool WICR_GameController::UpdateMassgate()
{
#if WIC_DS
	return ((bool(__thiscall *)(WICR_GameController *))0x0054FFE0)(this);
#else
	MC_ASSERTMSG(false, "Unreachable client code path");
	__debugbreak();
#endif
}