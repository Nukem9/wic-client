#include "MC_Debug.h"

MC_Debug::InternalPosTracer::InternalPosTracer(TraceType aType, const char *aFile, const char *aFunction, u32 aLine) :
	myType(aType),
	myFile(aFile),
	myFunction(aFunction),
	myLine(aLine)
{
}

void __declspec(naked) MC_Debug::InternalPosTracer::operator()(const char* aDebugMessage, ...)
{
	const static u32 dwJump = SelectOffset(0x00A02800, 0x00421180);

#if WIC_MP || WIC_DS
	// Bypass the variadic argument nonsense by directly jumping to game code instead
	__asm jmp [dwJump]
#else
#error Unimplemented
#endif
}

bool MC_Debug::AddDebugListener(MC_DebugListener *aListener)
{
#if WIC_MP
	return ((bool(__thiscall *)(MC_DebugListener *))0x00A01780)(aListener);
#elif WIC_DS
	return ((bool(__cdecl *)(MC_DebugListener *))0x0041EEC0)(aListener);
#else
#error Unimplemented
#endif
}
