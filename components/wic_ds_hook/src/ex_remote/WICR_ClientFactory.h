#pragma once

#pragma pack(push, 1)
class WICR_ClientFactory
{
public:
	bool ListenForClients()
	{
		// WICR_ClientFactory::ListenForClients
		return ((bool (__thiscall *)(WICR_ClientFactory *))0x0054FF20)(this);
	}
};
#pragma pack(pop)