#pragma once

#include <WinSock2.h>

class MN_UdpSocket
{
public:
	sockaddr_in myDestinationAddress = {};
	SOCKET myWinsock = INVALID_SOCKET;
	char myLocalHostName[260] = {};
	bool myHasSentBufferFlag = false;

	static MN_UdpSocket* Create(const bool anEnableBroadcastFlag);
	static void Destroy(MN_UdpSocket *&aSocket);

	virtual ~MN_UdpSocket();
	bool Bind(const u16 aPortNumber);
	virtual bool SendBuffer(const SOCKADDR_IN& aTargetAddress, const void *aBuffer, s32 aBufferLength);
	bool RecvBuffer(SOCKADDR_IN& aSourceAddress, char *aBuffer, s32 aBufferLength, s32& aNumReadBytes);

private:
	MN_UdpSocket() = default;
};