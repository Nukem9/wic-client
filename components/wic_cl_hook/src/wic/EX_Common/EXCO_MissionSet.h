#pragma once

#include <MCommon2/MC_GrowingArray.h>

class EXCO_MissionMetaInfo;

class EXCO_MissionSet
{
public:
	MC_GrowingArray<EXCO_MissionMetaInfo *> myMissions;
	MC_GrowingArray<EXCO_MissionMetaInfo *> myRandomMissions;
	EXCO_MissionMetaInfo *myLastRandomMission;
};
static_assert_size(EXCO_MissionSet, 0x24);