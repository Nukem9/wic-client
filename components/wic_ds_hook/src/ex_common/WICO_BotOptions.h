#pragma once

#pragma pack(push, 1)
class WICO_BotOptions : public EXCO_Persistent<WICO_BotOptions>
{
public:
	enum BotMode : int
	{
		BOTMODE_NONE,
		BOTMODE_EVEN,
		BOTMODE_PVSB,
		BOTMODE_ADVANCED,
	};

	class AutoEvenTeamOptions 
	{
	public:
		unsigned int			myNumPlayersPerTeam;	// this+0x0
		EXCO_Difficulty::Level	myBotDifficulty;		// this+0x4
	};

	class PvsBOptions 
	{
	public:
		WICO_Team::AllowedTeam	myBotTeam;			// this+0x0
		unsigned int			myNumBots;			// this+0x4
		EXCO_Difficulty::Level	myBotDifficulty;	// this+0x8
	};

	class AdvancedOptions 
	{
	public:
		class Bot 
		{
		public:
			bool					myIsActiveFlag;		// this+0x0
			EXCO_Difficulty::Level	myDifficulty;		// this+0x4
			unsigned int			myRoleId;			// this+0x8
			unsigned int			myAIDefinitionIndex;// this+0xC
		};

		AdvancedOptions::Bot myTeam1Bots[8];// this+0x0
		AdvancedOptions::Bot myTeam2Bots[8];// this+0x80
	};

	WICO_BotOptions::BotMode	myBotMode;				// this+0x1C
	AutoEvenTeamOptions			myAutoEvenTeamOptions;	// this+0x20
	PvsBOptions					myPvsBOptions;			// this+0x28
	AdvancedOptions				myAdvancedOptions;		// this+0x34
};
#pragma pack(pop)