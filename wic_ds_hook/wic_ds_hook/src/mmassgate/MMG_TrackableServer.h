#pragma once

class MMG_TrackableServer
{
public:
	CLASSPTR(MMG_TrackableServer *, 0x008F2CF0, ourInstance);

	static MMG_TrackableServer *GetInstance();

	bool Update();
};