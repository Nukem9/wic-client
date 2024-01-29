#include "MN_TcpSocket.h"

void MN_TcpSocket::Update()
{
#if WIC_DS
	((void(__cdecl *)())0x00439C40)();
#else
	MC_ASSERTMSG(false, "Unreachable client code path");
	__debugbreak();
#endif
}