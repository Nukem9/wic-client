#pragma once

#pragma pack(push, 1)
class EXCO_MissionSet 
{
public:
	MC_GrowingArray<EXCO_MissionMetaInfo *>	myMissions;				// this+0x0
	MC_GrowingArray<EXCO_MissionMetaInfo *>	myRandomMissions;		// this+0x10
	EXCO_MissionMetaInfo					*myLastRandomMission;	// this+0x20
};
#pragma pack(pop)