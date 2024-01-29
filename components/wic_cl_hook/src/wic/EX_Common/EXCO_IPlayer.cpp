#include "EXCO_MissionInfo.h"
#include "EXCO_IPlayer.h"

EXCO_PlayerInfo& EXCO_IPlayer::GetInfo() const
{
	if (myId == 255)
		return myMission.myScriptPlayerInfo;

	return myMission.myPlayers[myId];
}