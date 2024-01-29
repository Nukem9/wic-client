#include <MCommon2/MI_Time.h>
#include <MMassgate/MMG_TrackableServer.h>
#include <MNetwork/MN_TcpSocket.h>
#include <MRender/MR_Render.h>
#include <EX_Common/EXCO_Map.h>
#include <EX_Remote/WICR_ClientFactory.h>
#include <EX_Remote/WICR_GameController.h>
#include <Windows.h>
#include "EXD_DedicatedServer.h"

#undef GetCurrentTime

bool EXD_DedicatedServer::Update()
{
	myNumFrames++;

	if (auto trackableServer = MMG_TrackableServer::GetInstance())
		trackableServer->Update();

	if (!myGameController)
	{
		MC_ERROR("Trying to update when myGameController hasn't been initialized!");
		return false;
	}

	if (myGameController->myGame && !myClientFactory->ListenForClients())
	{
		MC_ERROR("myClientFactory->ListenForClients() failed");
		return false;
	}

	if (myGracefulShutdownTime > 0.0f)
	{
		if (MI_Time::GetCurrentTime() >= myGracefulShutdownTime)
		{
			MC_DEBUG("Initiating automatic shutdown");
			return false;
		}

		UpdateShutdownState();
	}

	if (!myGameController->UpdateMassgate())
	{
		MC_ERROR("myGameController->UpdateMassgate() failed");
		return false;
	}

	// Receive data from clients
	if (!myClientFactory->Receive())
	{
		MC_ERROR("myClientFactory->Receive() failed");
		return false;
	}

	// Run a single game tick and create the game instance if it doesn't exist
	if (!myGameController->Update())
	{
		MC_ERROR("myGameController->Update() failed");
		return false;
	}

	// Send data back to the clients
	if (!myClientFactory->Send(myGameController->myGame))
	{
		MC_ERROR("myClientFactory->Send() failed");
		return false;
	}

	if (EXCO_Map::ourInstance)
		EXCO_Map::ourInstance->Update();

	if (MR_Render::ourInstance)
	{
		if (MR_Render::ourInstance->BeginDraw(true))
			MR_Render::ourInstance->EndDraw(true);
	}

	PrintStats();
	MN_TcpSocket::Update();
	return true;
}

void EXD_DedicatedServer::UpdateShutdownState()
{
	((void(__thiscall *)(EXD_DedicatedServer *))0x00402CB0)(this);
}

void EXD_DedicatedServer::PrintStats()
{
	if (MI_Time::GetCurrentTime() >= myNextDebugTime)
	{
		char title[1024];

		if (myGameController->myGame)
		{
			sprintf_s(title, "WiC DS - %ls (%d units). %d:%02d remaining. Massgate: %s",
				myGameController->myGameInitData->myGameName.GetBuffer(),
				0,
				0,
				0,
				myGameController->myGameInitData->myReportToMassgateFlag ? "Online" : "Disabled");
		}
		else
		{
			sprintf_s(title, "WiC DS - %ls. Massgate: %s",
				myGameController->myGameInitData->myGameName.GetBuffer(),
				myGameController->myGameInitData->myReportToMassgateFlag ? "Online" : "Disabled");
		}

		SetConsoleTitleA(title);
	}

	((void(__thiscall *)(EXD_DedicatedServer *))0x00405370)(this);
}