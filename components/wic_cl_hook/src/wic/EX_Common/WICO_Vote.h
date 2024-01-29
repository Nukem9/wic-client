#pragma once

#include <MCommon2/MC_String.h>

class WICO_Vote
{
public:
	enum VoteType : s32
	{
		VOTE_NO_SPEC = 0x0,
		VOTE_ALLOW_SPEC = 0x1,
		VOTE_LOCK_FREE_SPEC = 0x2,
		VOTE_UNLOCK_FREE_SPEC = 0x3,
		VOTE_LOCK_TEAM_FROM_SPEC = 0x4,
		VOTE_UNLOCK_TEAM_FROM_SPEC = 0x5,
		VOTE_SET_MATCH_MODE = 0x6,
		VOTE_EXIT_MATCH_MODE = 0x7,
		VOTE_LOCK_TEAMS = 0x8,
		VOTE_UNLOCK_TEAMS = 0x9,
		VOTE_KICK_BAN_CLIENT_FROM_SERVER = 0xA,
		VOTE_PAUSE = 0xB,
		VOTE_RESUME = 0xC,
		VOTE_KICK_CLIENT_FROM_SERVER = 0xD,
		VOTE_CHANGE_MAP = 0xE,
		VOTE_LOCK_CHAT_FROM_SPEC = 0xF,
		VOTE_UNLOCK_CHAT_FROM_SPEC = 0x10,
		VOTE_TIME_LIMIT = 0x11,
		VOTE_INVITE_PLAYER = 0x12,
		SERVER_VOTE_GAME_OVER = 0x13,
		VOTE_NO_VOTE = 0x14,
		VOTE_NUM_TYPES = 0x15,
	};

	struct VoteValue
	{
		u32 myUint;
		s32 myInt;
		float myFloat;
		MC_String myString;
	};
	static_assert_size(VoteValue, 0x10);

	struct VoteResult
	{
		bool myHasVoted;
		bool myVotedYes;
	};
	static_assert_size(VoteResult, 0x2);
};