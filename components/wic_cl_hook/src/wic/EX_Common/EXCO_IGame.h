#pragma once

#include <MCommon2/MC_Array.h>
#include <MCommon2/MC_GrowingArray.h>
#include <MCommon2/MC_Vector.h>
#include "EXCO_Constants.h"
#include "EXCO_Behaviour.h"
#include "WICO_Vote.h"
#include "WICO_Request.h"

class EXCO_IPlayer;
class EXCO_PlayerInfo;
class EXCO_UnitType;
class WICO_StatsManager;
class WICO_BotOptions;

class EXCO_IGame
{
private:
	bool myIsSinglePlayerGameFlag;
	bool myIsHostShuttingDownGameFlag;

public:
	virtual ~EXCO_IGame() = 0;
	virtual bool CreateUnit(EXCO_IPlayer&, const EXCO_UnitType&, const MC_Vector3f&, const float) = 0;
#if WIC_DS
	virtual void Hack_SetUnitTeam(u16, s32) = 0;
	virtual void Hack_SetUnitOwner(u16, const EXCO_PlayerInfo&) = 0;
	virtual void Hack_KillUnit(u16) = 0;
	virtual void Hack_RemoveUnit(u16) = 0;
	virtual void CheatAddTA(EXCO_IPlayer&, const s32) = 0;
	virtual void CheatAddTAToAll(const s32) = 0;
#endif // WIC_DS
	virtual void Team1WinsGame() = 0;
	virtual bool IsRemote() = 0;
	virtual bool PlayerJoinTeam(EXCO_IPlayer&, s32, bool, bool) = 0;
	virtual bool SpectatorJoinTeam(EXCO_IPlayer&, const s32) = 0;
	virtual bool UseSpecialAbility(EXCO_IPlayer&, s16, s32) = 0;
	virtual bool UseSpecialAbility(EXCO_IPlayer&, s16, s32, MC_Vector3f, MC_Vector3f) = 0;
	virtual bool UseSpecialAbilityOnUnit(EXCO_IPlayer&, s16, s32, s16) = 0;
	virtual bool UseSpecialAbilityOnBuilding(EXCO_IPlayer&, s16, s32, s32) = 0;
	virtual bool UseSpecialAbilityOnProp(EXCO_IPlayer&, s16, s32, s32) = 0;
	virtual bool EndProcessingSpecialAbility(EXCO_IPlayer&, s16, s32) = 0;
	virtual bool MakeVote(EXCO_IPlayer&, WICO_Vote::VoteType, WICO_Vote::VoteValue) = 0;
	virtual bool Vote(EXCO_IPlayer&, bool) = 0;
	virtual bool ChangeMap(const char *) = 0;
	virtual bool ChangeTimeLimit(const s32) = 0;
	virtual bool InvitePlayerToTeam(const u8, const s32) = 0;
	virtual bool PlayerSetsReady(const char *) = 0;
	virtual bool ExitMatchMode(const char *) = 0;
	virtual bool GiveTAToAlly(EXCO_IPlayer&, s32, u8) = 0;
	virtual bool PlayerSetMatchReady(EXCO_IPlayer&) = 0;
	virtual bool DeployStartingUnits(EXCO_IPlayer&, MC_GrowingArray<EXCO_UnitType *>&) = 0;
	virtual bool SendChat(EXCO_IPlayer&, const wchar_t *, const bool) = 0;
	virtual bool SendChatPrivate(EXCO_IPlayer&, const wchar_t *, const EXCO_PlayerInfo *) = 0;
	virtual bool Stop(EXCO_IPlayer&, const u16) = 0;
	virtual bool StopSquad(EXCO_IPlayer&, const u16) = 0;
	virtual bool MoveFormationToPosition(EXCO_IPlayer&, const MC_GrowingArray<u16>&, const MC_GrowingArray<MC_Vector2f>&, const MC_GrowingArray<float>&, const float, const bool, const bool, const s16, bool) = 0;
	virtual bool MoveSquadToPosition(EXCO_IPlayer&, const u32, const MC_Vector3f&, const float, const float, const bool, const bool, const s16) = 0;
	virtual bool MoveToPosition(EXCO_IPlayer&, const u16, const MC_Vector3f&, const float, const float, const bool, const bool, const s16) = 0;
	virtual bool MoverFollow(EXCO_IPlayer&, const u16, const u16, const float) = 0;
	virtual bool SetPatrolRoute(EXCO_IPlayer&, const u16, const MC_Array<MC_Vector3f>&, float, float, bool, bool) = 0;
	virtual bool BroadcastLongMove(EXCO_IPlayer&, const u16, const MC_Vector2f&) = 0;
	virtual bool DirectAttackShoot(EXCO_IPlayer&, const u16, bool) = 0;
	virtual bool DirectAttackAim(EXCO_IPlayer&, const u16, const MC_Vector3f&) = 0;
	virtual bool DirectAttackAimUnit(EXCO_IPlayer&, const u16, const u16) = 0;
	virtual bool AttackPosition(EXCO_IPlayer&, const u16, const MC_Vector3f&, const bool) = 0;
	virtual bool AttackUnit(EXCO_IPlayer&, const u16, const u16, const bool) = 0;
	virtual bool AttackUnitWithSquad(EXCO_IPlayer&, const u16, const u16) = 0;
	virtual bool AttackUnits(EXCO_IPlayer&, const u16, MC_Array<u16>&, const bool, const bool) = 0;
	virtual bool AttackUnitsAsSubGroup(EXCO_IPlayer&, MC_GrowingArray<u16>&, MC_GrowingArray<u16>&, const bool, const bool) = 0;
	virtual bool AttackBuildings(EXCO_IPlayer&, const u16, MC_Array<s32>&, const bool, const bool) = 0;
	virtual bool StopAttacking(EXCO_IPlayer&, const u16) = 0;
	virtual bool SetDirectFireMode(EXCO_IPlayer&, const u16, bool) = 0;
	virtual bool SwitchDirectFireUnit(EXCO_IPlayer&, const u16) = 0;
	virtual bool UnitClearOrderQueue(EXCO_IPlayer&, const u16) = 0;
	virtual bool ResupplySquad(EXCO_IPlayer&, const u16) = 0;
	virtual bool CancelResupplySquad(EXCO_IPlayer&, const u16) = 0;
	virtual bool BuildingAttackUnits(EXCO_IPlayer&, const s32, MC_Array<u16>&) = 0;
	virtual bool BuildingAttackBuilding(EXCO_IPlayer&, const s32, const s32) = 0;
	virtual bool SquadLoadContainer(EXCO_IPlayer&, const u16, const u16, const bool) = 0;
	virtual bool ContainerLoad(EXCO_IPlayer&, const u16, const u16, const bool) = 0;
	virtual bool ContainerUnload(EXCO_IPlayer&, const u16, const u16, const bool) = 0;
	virtual bool ContainerUnloadAll(EXCO_IPlayer&, const u16, const bool) = 0;
	virtual bool ResidentEnterBuilding(EXCO_IPlayer&, const u16, const s32, bool, bool) = 0;
	virtual bool EnterDirectControlModeForUnit(EXCO_IPlayer&, const u16) = 0;
	virtual bool SpawnerSetPayload(EXCO_IPlayer&, MC_GrowingArray<EXCO_UnitType *>&) = 0;
	virtual bool SpawnerSetSpawnPosition(EXCO_IPlayer&, const MC_Vector3f&) = 0;
	virtual bool SpawnerSetRallyPoint(EXCO_IPlayer&, const MC_Vector3f&) = 0;
	virtual bool SpawnerStartSpawn(EXCO_IPlayer&, const u8) = 0;
	virtual bool BlowerBlow(EXCO_IPlayer&, const u16) = 0;
	virtual bool RepairerRepair(EXCO_IPlayer&, const u16, const u16, bool) = 0;
	virtual bool LayBridge(EXCO_IPlayer&, const u16, MC_Vector3f, float, bool) = 0;
	virtual bool SetUnitFireBehaviour(EXCO_IPlayer&, const u16, const EXCO_Behaviour::Fire) = 0;
	virtual bool SetUnitOwner(EXCO_IPlayer&, const u16, const EXCO_PlayerInfo&) = 0;
	virtual bool UseSupportThing(EXCO_IPlayer&, const u32, const MC_Vector3f&, MC_Vector3f) = 0;
	virtual bool StartSupportThingCooldown(EXCO_IPlayer&, const u32) = 0;
	virtual bool MapSignal(EXCO_IPlayer&, const MC_Vector3f&) = 0;
	virtual bool ExitGame(EXCO_IPlayer&) = 0;
	virtual bool MessageBoxClosed(EXCO_IPlayer&, const s32) = 0;
	virtual bool EventStringDone(EXCO_IPlayer&, const s32) = 0;
	virtual bool PlayerHaveFinishedLoading(EXCO_IPlayer&) = 0;
	virtual bool PlayerAction(EXCO_IPlayer&, const EXCO_Constants::PlayerAction) = 0;
	virtual bool PlayerEvent(EXCO_IPlayer&, const char *, const class _object *) = 0;
	virtual bool SetPauseState(EXCO_IPlayer&, const bool) = 0;
	virtual bool PlayerSetRole(EXCO_IPlayer&, const s32, bool) = 0;
	virtual bool PlayerSetESLFreeRole(EXCO_IPlayer&, const bool) = 0;
	virtual bool LosTestForAI(const MC_Vector3f&, const MC_Vector3f&, const u32, const u32) = 0;
	virtual void SendAIMsgCPMissionAquired(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgCPMissionEnded(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgAirEscortNeed(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgAirEscortMissonAquired(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgAirEscortReceived(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgHealingNeed(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgHealingMissionAquired(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgHealingReceived(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgFireSuppotNeed(EXCO_IPlayer&, const MC_Vector3f) = 0;
	virtual void SendAIMsgFireSuppotMissionAquired(EXCO_IPlayer&, const MC_Vector3f) = 0;
	virtual void SendAIMsgFireSuppotReceived(EXCO_IPlayer&, const MC_Vector3f) = 0;
	virtual void SendAIMsgBackupNeed(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgBackupMissionAquired(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgBackupReceived(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgTANeed(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgTAMissionAquired(EXCO_IPlayer&, const s32) = 0;
	virtual void SendAIMsgTAReceived(EXCO_IPlayer&, const s32) = 0;
	virtual void SpecialDebugPrintOutput() = 0;
	virtual bool IsSinglePlayerGame() = 0;
	virtual void ResetAllClouds() = 0;
	virtual bool CallPythonFunction(const char *, const s32) = 0;
	virtual bool CallPythonFunction(const char *) = 0;
	virtual void SendRequest(EXCO_IPlayer&, WICO_Request::RequestType, s32, const MC_Vector2f&, u32, const wchar_t *) = 0;
	virtual void AcceptRequest(EXCO_IPlayer&, u32) = 0;
	virtual void RestartSP() = 0;
	virtual void RestartAndLoadSP() = 0;
	virtual bool SetPlayerLANName(EXCO_IPlayer&, const wchar_t *) = 0;
	virtual void SendCameraPosition(EXCO_IPlayer&, const MC_Vector3f) = 0;
	virtual void SendCameraRotation(EXCO_IPlayer&, const MC_Vector2f) = 0;
	virtual void SendCameraHeightOverGround(EXCO_IPlayer&, const float) = 0;
	virtual bool DisbandUnit(EXCO_IPlayer&, u16) = 0;
	virtual WICO_StatsManager *GetStats() = 0;
	virtual bool CreateGameMode() = 0;
	virtual WICO_BotOptions *GetBotOptions() = 0;
	virtual void AdminPassword(EXCO_IPlayer&, MC_LocString) = 0;
	virtual void EmptyMessage(EXCO_IPlayer&) = 0;
};
static_assert_size(EXCO_IGame, 0x8);