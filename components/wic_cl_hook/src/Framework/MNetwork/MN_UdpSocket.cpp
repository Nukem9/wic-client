#include "MN_UdpSocket.h"

MN_UdpSocket *MN_UdpSocket::Create(const bool anEnableBroadcastFlag)
{
#if WIC_MP
	return ((MN_UdpSocket *(__cdecl *)(const bool))0x009D6290)(anEnableBroadcastFlag);
#elif WIC_DS
	return ((MN_UdpSocket *(__cdecl *)(const bool))0x0043B580)(anEnableBroadcastFlag);
#else
#error Unimplemented
#endif
}

void MN_UdpSocket::Destroy(MN_UdpSocket*& aSocket)
{
	delete aSocket;
	aSocket = nullptr;
}

/*
MN_UdpSocket::~MN_UdpSocket()
{
#error Unimplemented
}
*/

bool MN_UdpSocket::Bind(const u16 aPortNumber)
{
#if WIC_MP
	const static u32 dwCall = 0x009D60D0;

	__asm
	{
		mov ax, [aPortNumber]
		push ecx
		call [dwCall]
	}
#elif WIC_DS
	return ((bool(__thiscall *)(MN_UdpSocket *, const u16))0x0043B230)(this, aPortNumber);
#else
#error Unimplemented
#endif
}

/*
bool MN_UdpSocket::SendBuffer(const SOCKADDR_IN& aTargetAddress, const void *aBuffer, s32 aBufferLength)
{
#if WIC_MP

#elif WIC_DS
	return ((bool(__thiscall *)(MN_UdpSocket *, const SOCKADDR_IN&, const void *, s32))0x0043B2C0)(this, aTargetAddress, aBuffer, aBufferLength);
#else
#error Unimplemented
#endif
}
*/

bool MN_UdpSocket::RecvBuffer(SOCKADDR_IN& aSourceAddress, char *aBuffer, s32 aBufferLength, s32& aNumReadBytes)
{
#if WIC_MP
	const static u32 dwCall = 0x009D5F50;

	__asm
	{
		push aNumReadBytes
		push aBuffer
		push aSourceAddress
		mov edx, aBufferLength
		mov eax, this
		call [dwCall]
	}
#elif WIC_DS
	return ((bool(__thiscall *)(MN_UdpSocket *, SOCKADDR_IN&, char *, s32, s32&))0x0043B3C0)(this, aSourceAddress, aBuffer, aBufferLength, aNumReadBytes);
#else
#error Unimplemented
#endif
}