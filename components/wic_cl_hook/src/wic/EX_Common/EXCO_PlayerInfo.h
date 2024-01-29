#pragma once

#include <MCommon2/MC_String.h>
#include <MCommon2/MC_GrowingArray.h>
#include <MCommon2/MC_Vector.h>

class EXCO_PlayerInfo
{
public:
	enum Type : s32
	{
		HUMAN = 0,
		AI = 1,
		SCRIPT = 2,
		NUM_TYPES = 3,
	};

	enum SpectatorLOS : s32
	{
		SPECLOS_NONE = 0,
		SPECLOS_TEAM = 1,
		SPECLOS_ALL = 2,
	};

	bool myReadyFlag;
	bool myHasEnteredFlag;
	Type myType;
	u32 myMassgateProfileId;
	u32 myMassgateExperience;
	u32 myMassgateClanId;
	u8 myMassgateRankInClan;
	//MMG_PCCProtocol::MassgateToClientGetPCC::PCCResponse myPlayerCreatedContent[2];
	char _pad[0x218];
	bool myIsAllowedToVoteFlag;
	s32 myTeam;
	MC_LocString myName;
	MC_String myAIConfigFile;
	MC_Vector3f myBasePosition;
	MC_GrowingArray<u32> myAdditonalAllowedUnits;
	bool myIsSpectatorFlag;
	s32 myPlayerRole;
	u32 myVoipId;
	SpectatorLOS mySpectatorLOS;
	s32 myNumTeamChangesLeft;
	bool myIsAdminFlag;
};
static_assert_size(EXCO_PlayerInfo, 0x270);