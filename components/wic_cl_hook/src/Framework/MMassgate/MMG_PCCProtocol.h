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
#include <MCommon2/MC_String.h>

class MN_ReadMessage;
class MN_WriteMessage;

class MMG_PCCProtocol
{
public:
	class ClientToMassgateGetPCC
	{
	public:
		ClientToMassgateGetPCC();

		void ToStream(MN_WriteMessage& aWm, bool aCallerIsDS = false);
		bool FromStream(MN_ReadMessage& aRm);

		void AddPCCRequest(u32 anId, u8 aType);

		void Clear();
		int Count() const { return myPCCRequests.size(); }

		class PCCRequest
		{
		public:
			PCCRequest() = default;

			PCCRequest(u32 anId, u8 aType)
				: myId(anId)
				, myType(aType)
			{
			}

			u32 myId = 0;
			u8 myType = 0;
		};

		MC_HybridArray<PCCRequest, 16> myPCCRequests;
	};

	class MassgateToClientGetPCC
	{
	public:
		MassgateToClientGetPCC();

		void ToStream(MN_WriteMessage& aWm, bool aCallerIsDS = false);
		bool FromStream(MN_ReadMessage& aRm);

		void AddPCCResponse(u32 anId, u8 aType, u32 aSeqNum, const char* anURL);

		void Clear();
		int Count() const { return myPCCResponses.size(); }

		class PCCResponse
		{
		public:
			PCCResponse() = default;

			PCCResponse(u32 anId, u8 aType, u32 aSeqNum, const char *anURL)
				: myId(anId)
				, myType(aType)
				, myURL(anURL)
				, mySeqNum(aSeqNum)
			{
			}

			u32 myId = 0;
			u32 mySeqNum = 0;
			u8 myType = 0;
			MC_StaticString<256> myURL;
		};

		MC_HybridArray<PCCResponse, 16> myPCCResponses;
	};
};

class MMG_IPCCMessagingListener
{
public:
	virtual void HandlePCCResponse(const MMG_PCCProtocol::MassgateToClientGetPCC& aResponse) = 0;
};

class MMG_IPCCContentReadyListener
{
public:
	// Make a copy of aPath if you want to keep it 
	// aPath point to a stack allocated object don't mess with it 
	virtual void HandlePCCContentReady(const u32 anId, const u8 aType, const char* aPath) = 0;
};