#pragma once

#include <EX_Common/EXCO_Constants.h>

class MVoip_Server;
class MVoip_AudioCodec;

class WICG_VoipServer
{
public:
	u32 aPlayerSlot;
	MVoip_Server *myServer;
	MVoip_AudioCodec *myServerCodecList[EX_MAX_NUM_CLIENTS];
	bool myIsInitedFlag;
};
static_assert_size(WICG_VoipServer, 0x4C);