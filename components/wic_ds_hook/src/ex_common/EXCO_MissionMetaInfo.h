#pragma once

#pragma pack(push, 1)
class EXCO_MissionMetaInfo 
{
	uint32_t			myStartingAP;				// this+0x0
	uint32_t			myStartingTickets;			// this+0x4
	uint32_t			myMaxAP;					// this+0x8
	MC_Str<char, 0>		myGameMode;					// this+0xC
	uint8_t				myDataMD5[40];				// this+0x10
	uint64_t			myTigerHash;				// this+0x38
	MC_Str<char, 0>		myFile;						// this+0x40
	MC_Str<wchar_t, 0>	myName;						// this+0x44
	MC_Str<wchar_t, 0>	mySPName;					// this+0x48
	MC_Str<wchar_t, 0>	myDescription;				// this+0x4C
	MC_Str<wchar_t, 0>	mySPDescription;			// this+0x50
	MC_Str<wchar_t, 0>	myMissionBrief;				// this+0x54
	MC_Str<char, 0>		myMinimapFile;				// this+0x58
	MC_Vector2<float>	myPlayfieldMinCoord;		// this+0x5C
	MC_Vector2<float>	myPlayfieldMaxCoord;		// this+0x64
	uint32_t			myAllowedTeam1;				// this+0x6C
	uint32_t			myAllowedTeam2;				// this+0x70
	MC_Str<char, 0>		myListName;					// this+0x74
	uint32_t			myRefCount;					// this+0x78
	bool				myFlaggedForMapCycleFlag;	// this+0x7C
};
#pragma pack(pop)