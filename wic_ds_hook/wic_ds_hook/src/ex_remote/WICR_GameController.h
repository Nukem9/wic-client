#pragma once

#pragma pack(push, 1)
class WICR_GameController
{
public:
	class WICR_GameInitData 
	{
		MC_Str<char, 0>			myMapCycleFile;		 // this+0x0
		MC_Str<char, 0>			myPublicIP;		 // this+0x4
		MC_Str<char, 0>			myCustomMapPath;		 // this+0x8
		MC_Str<wchar_t, 0>		myGameName;		 // this+0xC
		MC_Str<wchar_t, 0>		myPassword;		 // this+0x10
		MC_Str<wchar_t, 0>		myAdminPassword;		 // this+0x14
		MC_Str<wchar_t, 0>		myMessageOfTheDay;		 // this+0x18
		MC_StaticString<512>	myBannerUrl;		 // this+0x1C
		int						myMaxPlayers;		 // this+0x21C
		int						myMaxNumAllowedTeamChanges;		 // this+0x220
		int						myNumAllowedPlayersPerRole;		 // this+0x224
		int						myMaxAllowedIdleTime;		 // this+0x228
		int						myChatTimeSpan;		 // this+0x22C
		int						myMaxChatMessages;		 // this+0x230
		int						myChatSpamMuteTime;		 // this+0x234
		int						myRequestTimeSpan;		 // this+0x238
		int						myMaxRequests;		 // this+0x23C
		int						myRequestSpamMuteTime;		 // this+0x240
		int						myFriendlyFireLimit;		 // this+0x244
		bool					myShowAdminTagFlag;		 // this+0x248
		int						myMinimumPlayersRequired;		 // this+0x24C
		int						myBanTime;		 // this+0x250
		MC_Str<char, 0>			myBanFileName;		 // this+0x254
		float					myTimeLimitMultiplier;		 // this+0x258
		ushort					myMassgateQueryPort;		 // this+0x25C
		ushort					myGamePortRangeStart;		 // this+0x25E
		ushort					myAdminPort;		 // this+0x260
		uint					myTournamentClanA;		 // this+0x264
		uint					myTournamentClanB;		 // this+0x268
		bool					myTournamentServerFlag;		 // this+0x26C
		bool					myClanMatchServerFlag;		 // this+0x26D
		bool					myIsESLMatchFlag;		 // this+0x26E
		bool					myReportToMassgateFlag;		 // this+0x26F
		bool					myRankedGameFlag;		 // this+0x270
		bool					myUseFireWallSettingsFlag;		 // this+0x271
		bool					myIsMatchMode;		 // this+0x272
		bool					myIsFPMMode;		 // this+0x273
		bool					myNonLinearDominationDisabled;		 // this+0x274
		int						myModId;		 // this+0x278
		bool					myRandomizeMissionOrderFlag;		 // this+0x27C
		bool					myUseAutoTeamBalanceFlag;		 // this+0x27D
		float					myAutoBalanceStartDelay;		 // this+0x280
		bool					myAllowSpectatorVotingFlag;		 // this+0x284
		bool					myAllowSpectatingFlag;		 // this+0x285
		bool					myAllowFreeSpectatingFlag;		 // this+0x286
		bool					myStayOnTeamFlag;		 // this+0x287
		bool					myRankBalanceTeams;		 // this+0x288
		uint					mySupplierId;		 // this+0x28C
		WICO_BotOptions			myBotOptions;		 // this+0x290
	};
};
#pragma pack(pop)