#include "../stdafx.h"

bool MN_Message::Empty()
{
	return this->myCurrentPacket == nullptr;
}