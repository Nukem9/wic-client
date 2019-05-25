#pragma once

#include "../mcommon2/MC_Str.h"

class WICO_Vote
{
public:
	class VoteValue
	{
		unsigned int			myUint;		// this+0x0
		int						myInt;		// this+0x4
		float					myFloat;	// this+0x8
		class MC_Str<char, 0>	myString;	// this+0xC
	};

	enum VoteType : int
	{
		VOTE_NO_SPEC = 0,
		VOTE_ALLOW_SPEC = 1,
		VOTE_LOCK_FREE_SPEC = 2,
		VOTE_UNLOCK_FREE_SPEC = 3,
		VOTE_LOCK_TEAM_FROM_SPEC = 4,
		VOTE_UNLOCK_TEAM_FROM_SPEC = 5,
		VOTE_SET_MATCH_MODE = 6,
		VOTE_EXIT_MATCH_MODE = 7,
		VOTE_LOCK_TEAMS = 8,
		VOTE_UNLOCK_TEAMS = 9,
		VOTE_KICK_BAN_CLIENT_FROM_SERVER = 10,
		VOTE_PAUSE = 11,
		VOTE_RESUME = 12,
		VOTE_KICK_CLIENT_FROM_SERVER = 13,
		VOTE_CHANGE_MAP = 14,
		VOTE_LOCK_CHAT_FROM_SPEC = 15,
		VOTE_UNLOCK_CHAT_FROM_SPEC = 16,
		VOTE_TIME_LIMIT = 17,
		VOTE_INVITE_PLAYER = 18,
		SERVER_VOTE_GAME_OVER = 19,
		VOTE_NO_VOTE = 20,
		VOTE_NUM_TYPES = 21,
	};

	class VoteResult
	{
		bool myHasVoted;	// this+0x0
		bool myVotedYes;	// this+0x1
	};

private:
	void *pvTable;
	VoteType			myVote;					// this+0x4
	const VoteValue		myValue;				// this+0x8
	const unsigned int	myVoter;				// this+0x18
	int					myAllowedTeams;			// this+0x1C
	const float			myStartTime;			// this+0x20
	float				myDuration;				// this+0x24
	float				myPlayerCooldown;		// this+0x28
	float				myYesWeight;			// this+0x2C
	float				myNoWeight;				// this+0x30
	float				myBlankWeight;			// this+0x34
	float				myKickTeamExtraWeight;	// this+0x38
	VoteResult			myVoteResults[16];		// this+0x3C
};