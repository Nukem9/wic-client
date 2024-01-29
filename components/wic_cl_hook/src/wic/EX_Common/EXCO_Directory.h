#pragma once

#include <Hooks/Memory.h>

class Ice;

class EXCO_Directory
{
private:
	char _padding[0x1C];
	const Ice *myDirectoryRoot;

public:
	static inline DeclareGlobalRef(EXCO_Directory *, ourInstance, SelectOffset(0x00E1F530, 0x008EF114));

	static const Ice *GetDirectoryRoot();
};