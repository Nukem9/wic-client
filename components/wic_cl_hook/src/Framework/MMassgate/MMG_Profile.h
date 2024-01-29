// Massgate
// Copyright (C) 2017 Ubisoft Entertainment
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// THIS FILE HAS BEEN MODIFIED FOR USE IN A NON-UBISOFT PROJECT. IT IS NOT
// AN ORIGINAL COPY.
//
#pragma once

#include "MMG_Constants.h"

class MN_ReadMessage;
class MN_WriteMessage;

// NOTE! The stats and medals defined here should match those in MMG_Stats, but this format is used to send
// complete info on a player to a client, the other format is a tighter format for DS stats reporting.

class MMG_Profile
{
public:
	MMG_Profile() = default;

	void ToStream(MN_WriteMessage& aWm) const;
	bool FromStream(MN_ReadMessage& aRm);

	bool operator==(const MMG_Profile& aRhs) const
	{
		return myProfileId == aRhs.myProfileId;
	}

	MMG_ProfilenameString myName;
	u32 myProfileId = 0;
	u32 myClanId = 0;			// 0=no clan, -1 = unknown, n=clanid
	u32 myOnlineStatus = 0;		// 0=offline, 1=online, x=playing on server id x
	u8 myRank = 0;				// 0=private, 1=sergeant, etc
	u8 myRankInClan = 0;		// 0=not in clan, 1=leader, 2=officer, 3=grunt
};
static_assert_size(MMG_Profile, 0x44);