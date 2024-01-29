#include "MC_HeapImplementation.h"

void *MC_Allocate(size_t aSize, const char *aFileName, int aLine, void *aThreadHandle, const void* aContext, bool anArrayFlag)
{
#if WIC_MP
	// MC_Allocate was totally optimized away in the client and broadcast tool. We'll fake it by calling
	// operator new instead.
	return ((void *(__cdecl *)(size_t))0x00B2DDE0)(aSize);
#elif WIC_DS
	return ((void *(__cdecl *)(size_t, const char *, int, void *, const void *, bool))0x00401DD0)(aSize, aFileName, aLine, aThreadHandle, aContext, anArrayFlag);
#else
#error Unimplemented
#endif
}

void MC_Free(void* aPointer, bool anArrayFlag)
{
#if WIC_MP
	// Same as MC_Allocate above. We'll fake it by calling operator delete instead.
	((void(__cdecl *)(void *))0x00B2DDA0)(aPointer);
#elif WIC_DS
	((void(__cdecl *)(void *))0x00401F80)(aPointer);
#else
#error Unimplemented
#endif
}