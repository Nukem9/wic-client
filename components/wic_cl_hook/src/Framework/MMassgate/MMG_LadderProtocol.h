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

#include <MCommon2/MC_HybridArray.h>
#include "MMG_Profile.h"

class MN_ReadMessage;
class MN_WriteMessage;

class MMG_LadderProtocol
{
public:
	class LadderReq
	{
	public:
		void ToStream(MN_WriteMessage& theStream) const;
		bool FromStream(MN_ReadMessage& theStream);

		u32 startPos = 0;
		u32 profileId = 0;
		u32 numItems = 0;
		u32 requestId = 0;
	};
	static_assert_size(LadderReq, 0x10);

	class LadderRsp
	{
	public:
		void ToStream(MN_WriteMessage& theStream) const;
		bool FromStream(MN_ReadMessage& theStream);

		void Add(const MMG_Profile& aProfile, u32 aScore);

		bool GotFullResponse() const
		{
			return myResponseIsFull;
		}

		class LadderItem
		{
		public:
			LadderItem() = default;

			LadderItem(const MMG_Profile& aProfile,
				u32 aScore)
				: profile(aProfile)
				, score(aScore)
			{
			}

			MMG_Profile profile;
			u32 score = 0;
		};

		MC_HybridArray<LadderItem, 100> ladderItems;

		u32 startPos = 0;
		u32 requestId = 0;
		u32 ladderSize = 0;
		bool myResponseIsFull = false;
	};
	static_assert_size(LadderRsp, 0x1C3C);
};
static_assert_size(MMG_LadderProtocol, 0x1);