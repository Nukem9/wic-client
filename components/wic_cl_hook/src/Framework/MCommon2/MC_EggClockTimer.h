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

#include "MI_Time.h"

class MC_EggClockTimer
{
public:
	MC_EggClockTimer(u32 aTimeoutMillis)
		: myTimeoutMillis(aTimeoutMillis)
		, myStartTimeMillis(MI_Time::GetSystemTime())
	{
	}

	MC_EggClockTimer() = default;
	~MC_EggClockTimer() = default;

	bool HasExpired()
	{
		const auto currentTimeMillis = MI_Time::GetSystemTime();

		if ((currentTimeMillis - myStartTimeMillis) >= myTimeoutMillis)
		{
			myStartTimeMillis = currentTimeMillis;
			return true;
		}

		return false;
	}

	void Reset()
	{
		myStartTimeMillis = MI_Time::GetSystemTime();
	}

	void SetTimeout(u32 aTimeoutMillis)
	{
		myStartTimeMillis = MI_Time::GetSystemTime();
		myTimeoutMillis = aTimeoutMillis;
	}

	u32 TimeLeft() const
	{
		return myTimeoutMillis - (MI_Time::GetSystemTime() - myStartTimeMillis);
	}

private:
	u32 myTimeoutMillis = 0;
	u32 myStartTimeMillis = 0;
};