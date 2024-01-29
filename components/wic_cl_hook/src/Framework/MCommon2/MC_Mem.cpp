#include "MC_Mem.h"

void* MC_TempAlloc(u32 aSize)
{
#if WIC_MP
	const static u32 dwCall = 0x009FEA00;

	__asm
	{
		mov eax, aSize
		call [dwCall]
	}
#elif WIC_DS
	return ((void *(__cdecl *)(u32))0x0041CDA0)(aSize);
#else
#error Unimplemented
#endif
}

void* MC_TempAllocIfOwnerOnStack(void* anOwner, u32 aSize, const char* aFile, int aLine)
{
#if WIC_MP
#if MC_TEMP_MEMORY_HANDLER_ENABLED
	if (MC_IsProbablyOnStack(anOwner))
		return MC_TempAlloc(aSize);
#endif

	// We're technically supposed to call operator new here, but can't globally override it. I suppose
	// MC_Allocate will be needed instead.
	__debugbreak();
	return new char[aSize];
#elif WIC_DS
	return ((void *(__cdecl *)(void *, u32, const char *, int))0x0041D160)(anOwner, aSize, aFile, aLine);
#else
#error Unimplemented
#endif
}

void MC_TempFree(void* aPointer)
{
#if WIC_MP
	return ((void(__cdecl *)(void *))0x009FE8F0)(aPointer);
#elif WIC_DS
	return ((void(__cdecl *)(void *))0x0041CED0)(aPointer);
#else
#error Unimplemented
#endif
}

bool MC_IsProbablyOnStack(const void* aPointer)
{
#if WIC_MP
	return ((bool(__fastcall *)(const void *))0x009FE620)(aPointer);
#elif WIC_DS
	return ((bool(__cdecl *)(const void *))0x0041D0F0)(aPointer);
#else
#error Unimplemented
#endif
}

bool MC_Assert(const char *aFile, int aLine, const char *aString, bool *aIgnoreFlag)
{
#if WIC_MP
	return ((bool(__cdecl *)(const char *, int, const char *, bool *))0x009FECF0)(aFile, aLine, aString, aIgnoreFlag);
#elif WIC_DS
	return ((bool(__cdecl *)(const char *, int, const char *, bool *))0x0041CAF0)(aFile, aLine, aString, aIgnoreFlag);
#else
#error Unimplemented
#endif
}