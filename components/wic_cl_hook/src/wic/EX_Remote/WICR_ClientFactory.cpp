#include "WICR_ClientFactory.h"

bool WICR_ClientFactory::ListenForClients()
{
#if WIC_MP
	const static u32 dwCall = 0x00772FB0;

	__asm
	{
		mov eax, this
		call [dwCall]
	}
#elif WIC_DS
	return ((bool(__thiscall *)(WICR_ClientFactory *))0x0054FF20)(this);
#else
#error Unimplemented
#endif
}

bool WICR_ClientFactory::Send(EXG_Game *aGame)
{
#if WIC_MP
	return ((bool(__stdcall *)(WICR_ClientFactory *, EXG_Game *))0x00772F00)(this, aGame);
#elif WIC_DS
	return ((bool(__thiscall *)(WICR_ClientFactory *, EXG_Game *))0x0054C970)(this, aGame);
#else
#error Unimplemented
#endif
}

bool WICR_ClientFactory::Receive()
{
#if WIC_MP
	return ((bool(__stdcall *)(WICR_ClientFactory *))0x00772D30)(this);
#elif WIC_DS
	return ((bool(__thiscall *)(WICR_ClientFactory *))0x0054D340)(this);
#else
#error Unimplemented
#endif
}