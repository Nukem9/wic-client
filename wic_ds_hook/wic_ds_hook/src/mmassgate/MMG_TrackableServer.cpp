#include "../stdafx.h"

DPTR(MMG_TrackableServer, ourInstance);

MMG_TrackableServer *MMG_TrackableServer::GetInstance()
{
	return ourInstance;
}

bool MMG_TrackableServer::Update()
{
	return ((bool (__thiscall *)(MMG_TrackableServer *))0x00482A30)(this);
}