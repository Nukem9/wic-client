#pragma once

class EXCO_MissionInfo;
class EXCO_PlayerInfo;
class WICO_Team;

class EXCO_IPlayer
{
public:
	virtual ~EXCO_IPlayer();
	// and a whole shitload of virtual functions...

	s32 myCurrentMaxAP;
	EXCO_MissionInfo& myMission;
	const u8 myId;
	u32 myAP;
	float myHonors;
	float myMaxHonors;
	s32 myRegrowthAP;
	u32 myAPPenalty;
	WICO_Team& myTeam;

	EXCO_PlayerInfo& GetInfo() const;
};
static_assert_size(EXCO_IPlayer, 0x28);