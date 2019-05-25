#pragma once

#include "../mcommon2/MC_Vector3.h"
#include "../mcommon2/MC_Str.h"
#include "../mcommon2/MC_GrowingArray.h"

#pragma pack(push, 4)
class EXCO_PlayerInfo
{
public:
	enum Type : int
	{
		HUMAN = 0,
		AI = 1,
		SCRIPT = 2,
		NUM_TYPES = 3,
	};

	enum SpectatorLOS : int
	{
		SPECLOS_NONE = 0,
		SPECLOS_TEAM = 1,
		SPECLOS_ALL = 2,
	};

private:
	bool							myReadyFlag;		 // this+0x0
	bool							myHasEnteredFlag;		 // this+0x1
	Type							myType;		 // this+0x4
	unsigned int					myMassgateProfileId;		 // this+0x8
	unsigned int					myMassgateExperience;		 // this+0xC
	unsigned int					myMassgateClanId;		 // this+0x10
	unsigned char					myMassgateRankInClan;		 // this+0x14
	//class MMG_PCCProtocol::MassgateToClientGetPCC::PCCResponse myPlayerCreatedContent[2];		 // this+0x18
	char _pad[0x218];
	bool							myIsAllowedToVoteFlag;		 // this+0x230
	int								myTeam;		 // this+0x234
	MC_Str<wchar_t, 0>				myName;		 // this+0x238
	MC_Str<char, 0>					myAIConfigFile;		 // this+0x23C
	MC_Vector3<float>				myBasePosition;		 // this+0x240
	MC_GrowingArray<unsigned int>	myAdditonalAllowedUnits;		 // this+0x24C
	bool							myIsSpectatorFlag;		 // this+0x25C
	int								myPlayerRole;		 // this+0x260
	unsigned int					myVoipId;		 // this+0x264
	SpectatorLOS					mySpectatorLOS;		 // this+0x268
	int								myNumTeamChangesLeft;		 // this+0x26C
	bool							myIsAdminFlag;		 // this+0x270

public:
	int GetTeam()
	{
		return ((int(__thiscall *)(EXCO_PlayerInfo *))0x0045BD50)(this);
	}

	int GetRole()
	{
		return myPlayerRole;
	}
};
#pragma pack(pop)