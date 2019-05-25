#pragma once

#include "WICO_Team.h"
#include "WICO_Vote.h"
#include "EXCO_LOSSet.h"
#include "EXCO_PlayerInfo.h"
#include "EXCO_MissionInfo.h"

#pragma pack(push, 4)
class EXCO_IPlayer
{
private:
	int	myCurrentMaxAP;				// this+0x4
	EXCO_MissionInfo& myMission;	// this+0x8
	const unsigned char	myId;		// this+0xC
	unsigned int myAP;				// this+0x10
	float myHonors;					// this+0x14
	float myMaxHonors;				// this+0x18
	int	myRegrowthAP;				// this+0x1C
	unsigned int myAPPenalty;		// this+0x20
	WICO_Team& myTeam;				// this+0x24

public:
	EXCO_PlayerInfo *GetInfo()
	{
		return ((EXCO_PlayerInfo *(__thiscall *)(EXCO_IPlayer *))0x0045C3B0)(this);
	}

	void EnforceAPLimits();
	void hk_UpdateAPPenalty(unsigned int aPenaltyDelta);
	bool hk_ChangeAP(const int aDelta);
	bool hk_SetAP(const int aStartVal, const int aRegrowthVal, const float aBattleHonor);
	bool hk_AddAPRegrowth(const int anAPAmount);
	int hk_GetAPInUse();

	virtual void *__vecDelDtor(unsigned int);
	virtual bool PlayerJoinedTeam(const char, const int, const int);
	virtual bool SpectatorJoinedTeam(const char, const int, EXCO_PlayerInfo::SpectatorLOS);
	virtual bool IsSpectator();
	virtual void SetIsSpectator(bool);
	virtual void SetSpectatorLOS(EXCO_PlayerInfo::SpectatorLOS);
	virtual EXCO_PlayerInfo::SpectatorLOS GetSpectatorLOS();
	virtual bool SendMatchReadyCheck();
	virtual bool MatchStarting(float);
	virtual bool PlayerSetsReady(const char);
	virtual bool MakeVote(const char, WICO_Vote::VoteType, WICO_Vote::VoteValue);
	virtual bool VoteEnded(WICO_Vote::VoteType, WICO_Vote::VoteValue, bool);
	virtual bool Vote(const char, const bool);
	virtual bool ChangeAP(const int);
	virtual bool ChangeHonors(const float);
	virtual bool ChangeRegrowth(const int);
	virtual bool AddAPRegrowth(const int);
	virtual bool SetAP(const int, const int, const float);
	virtual bool SetMaxAP(unsigned int);
	virtual bool SetCurrentAP(const int);
	virtual bool SetAPPenalty(const int, bool);
	virtual bool EnableSpecialAbility(__int16, int);
	virtual bool DisableSpecialAbilities(__int16, bool);
	virtual bool ResetSpecialAbility(__int16, int, float);
	virtual bool SpecialAbilityStartProcessing(__int16, int, float);
	virtual bool SpecialAbilityPending(__int16, int);
	virtual bool PlayerError(const char *);
	virtual bool UpdateLOS(EXCO_LOSSet *);
	virtual bool PlayerEntersGame(const char, EXCO_PlayerInfo *);
	virtual bool PlayerLeavesGame(const char, const float);
	virtual bool PlayerReceiveChat(EXCO_PlayerInfo *, const wchar_t *, bool);
	virtual bool PlayerReceiveChatPrivate(EXCO_PlayerInfo *, const wchar_t *);
	virtual bool RequestStartingUnits();
	virtual bool BlinkUnit(const unsigned __int16, bool, float, float);
	virtual bool ResumeDispand(const unsigned __int16);
	virtual bool ProjectileStraightCreate(const unsigned int, MC_Vector3<float> *, MC_Vector3<float> *, const unsigned __int16, const unsigned __int16);
	virtual bool ProjectileBallisticCreate(const unsigned int, MC_Vector3<float> *, MC_Vector3<float> *, const unsigned __int16, const unsigned __int16);
	virtual bool ProjectileHomingCreate(const unsigned int, MC_Vector3<float> *, MC_Vector3<float> *, const unsigned __int16, const float, const unsigned __int16, const unsigned __int16);
	virtual bool ProjectileHomingCreate(const unsigned int, MC_Vector3<float> *, MC_Vector3<float> *, const unsigned __int16, const float, MC_Vector3<float> *, const unsigned __int16);
	/*
	virtual bool ProjectileStraightSupportCreate(const unsigned int, EXCO_SupportThing *, MC_Vector3<float> *, MC_Vector3<float> *, unsigned int, const char *);
	virtual bool ProjectileBallisticSupportCreate(const unsigned int, EXCO_SupportThing *, MC_Vector3<float> *, MC_Vector3<float> *, unsigned int, const char *);
	virtual bool ProjectileHomingSupportCreate(const unsigned int, EXCO_SupportThing *, MC_Vector3<float> *, MC_Vector3<float> *, const float, MC_Vector3<float> *, unsigned int);
	virtual bool ProjectileHomingSupportCreate(const unsigned int, EXCO_SupportThing *, MC_Vector3<float> *, MC_Vector3<float> *, const float, const unsigned __int16, unsigned int, const char *);
	virtual bool DestroyAllProjectiles();
	virtual bool CreateGenericModelFromScript(const char *, MC_Vector3<float>, const float, MC_Vector3<float>, const char *, const char *, int);
	virtual bool DeleteGenericModel(int);
	virtual bool AntiProjectile(const unsigned int);
	virtual bool AddCommandPoint(int, const char *, MC_Vector3<float> *, int, bool);
	virtual bool AddPerimeterPoint(int, int, MC_Vector3<float> *, float, int);
	virtual bool SetCommandPointCapturable(const int, const bool);
	virtual bool SetCommandPointOwner(const int, const int);
	virtual bool SetPerimeterPointOwner(const int, const int);
	virtual bool SetCommandPointActive(const int, const bool);
	virtual bool SetRoundInfo(MC_GrowingArray<float> *, MC_GrowingArray<float> *);
	virtual bool AddCNMH_Zone(const int, const int, const int);
	virtual bool StartCNMH_Timer(const int, const int);
	virtual bool StartCNMH_Arrival(const int, const int);
	virtual bool StartCNMH_Delivery(const int, const int);
	virtual bool AbortCNMH(const int, const int);
	virtual bool UpdateCNMH_BuildPercentage(const int, const char, const bool);
	virtual bool AddOffensiveFortificationPoint(MC_Vector3<float>, const int, const int);
	virtual bool OffensiveFortificationSetOwner(const int, EXCO_PlayerInfo *);
	virtual bool PlayerChooseRole(const float);
	virtual bool PlayerSetRole(const char, const int);
	virtual bool PlayerSetESLFreeRole(const char, const bool);
	virtual bool BuildingCreate(const int, const int, MC_Vector3<float> *, MC_Vector3<float> *, EXCO_Slot::RotatedRep *, EXCO_Constants::DestroyableState, const int, const unsigned int);
	virtual bool BuildingDamaged(const int, const int, EXCO_Constants::DestroyableState, bool);
	virtual bool BuildingSetSlotState(int, int, bool, unsigned __int16);
	virtual bool PropDamaged(const int, EXCO_Constants::DestroyableState, bool);
	virtual bool RepairablePropDamaged(const int, EXCO_Constants::DestroyableState, const int, const int, bool);
	virtual bool AllowJoinTeamScreenWithStayOnTeam(bool);
	virtual bool DeployableCreate(const int, EXCO_PlayerInfo *, const int, MC_Vector3<float>, const float, const int);
	virtual bool DeployableDamaged(const int, const int);
	virtual bool SpawnExplosion(MC_Vector3<float>, float, float, float, float);
	virtual bool SpawnExplosionWithCrater(MC_Vector3<float>, float, float, float, float, __int16);
	virtual bool UnitCreate(const unsigned int, EXCO_PlayerInfo *, const int, const unsigned __int16, EXCO_UnitType *, MC_Vector3<float> *, const float, const int, EXCO_Behaviour::Fire, const float, const char, const int, const int, const char, bool, EXCO_Constants::SpawnSource);
	virtual bool UnitRemove(const unsigned __int16, const bool);
	virtual bool UnitDestroy(WICO_UnitKillData *);
	virtual bool StartUnitFrames();
	virtual bool UnitFrame(const unsigned __int16, MC_Vector3<float> *, const float, const float, const float, const float, const char, EXCO_Constants::WIC_ShortShooterData *);
	virtual bool UnitHealth(const unsigned __int16, const int, EXCO_Direction::Direction);
	virtual bool UnitSetOwner(const unsigned __int16, EXCO_PlayerInfo *);
	virtual bool UnitSetTeam(const unsigned __int16, const int);
	virtual bool UnitSetImmovable(const unsigned __int16, const char);
	virtual bool SetState_Unit(const unsigned __int16, const int, const float);
	virtual bool AddAdditionalUnitType(const unsigned int);
	virtual bool RemoveAdditionalUnitType(const unsigned int);
	virtual bool BroadcastLongMove(const unsigned __int16, MC_Vector2<float> *);
	virtual bool SendTATaunt(char, char, unsigned int, unsigned int);
	virtual bool SetDirectControlMode(const char, const unsigned __int16, const bool);
	virtual bool DirectControlFailed(const unsigned __int16);
	virtual bool DirectControlSwitched(const char, const unsigned __int16);
	virtual bool DirectControlSwitchFailed(const unsigned __int16);
	virtual bool AddLOSArea(WICO_LOSArea);
	virtual bool SetLOSAreaTimeToDie(const int, const float);
	virtual bool ClearOrderQueue(const unsigned __int16);
	virtual bool MoverIsMoving(const unsigned __int16, MC_Vector3<float> *, const float, const bool);
	virtual bool MoverCantMove(const unsigned __int16, MC_Vector3<float> *, const float);
	virtual bool MoverIsFollowing(const unsigned __int16, const unsigned __int16);
	virtual bool MoverCantFollow(const unsigned __int16, const unsigned __int16);
	virtual bool MoverHalted(const unsigned __int16);
	virtual bool UnitStopped(const unsigned __int16);
	virtual bool UnitCantStop(const unsigned __int16);
	virtual bool MoverQueued(const unsigned __int16, MC_Vector3<float> *, const float, const float);
	virtual bool ShooterIsAttacking(const unsigned __int16, MC_Array<unsigned short> *);
	virtual bool ShooterIsAttacking(const unsigned __int16, const unsigned __int16);
	virtual bool ShooterIsAttacking(const unsigned __int16, MC_Vector3<float> *);
	virtual bool ShooterCantAttack(const unsigned __int16, MC_Array<unsigned short> *);
	virtual bool ShooterCantAttack(const unsigned __int16, const unsigned __int16);
	virtual bool ShooterCantAttack(const unsigned __int16, MC_Vector3<float> *);
	virtual bool ShooterAcquiredTarget(const unsigned __int16, const unsigned __int16);
	virtual bool SwitchShooter(const unsigned __int16, const char);
	virtual bool ShooterQueuedAttack(const unsigned __int16, MC_Vector3<float> *);
	virtual bool SpawnerSetPayload(const unsigned __int16);
	virtual bool SpawnerCantSetPayload(const unsigned __int16);
	virtual bool ContainerWillLoad(const unsigned __int16, const unsigned __int16, MC_Vector3<float> *);
	virtual bool ContainerCantLoad(const unsigned __int16, const unsigned __int16);
	virtual bool ContainerCantUnload(const unsigned __int16, const unsigned __int16);
	virtual bool ContainerCantUnloadAll(const unsigned __int16);
	virtual bool UnitWillLoadQueued(const unsigned __int16, const unsigned __int16);
	virtual bool ContainerWillUnloadAllQueued(const unsigned __int16);
	virtual bool ResidentWillEnterBuilding(const unsigned __int16, const int, MC_Vector3<float> *);
	virtual bool ResidentCantEnterBuilding(const unsigned __int16, const int);
	virtual bool ResidentWillEnterBuildingQueued(const unsigned __int16, const int);
	virtual bool BlowerBlew(const unsigned __int16);
	virtual bool RepairerWillRepair(const unsigned __int16, const unsigned __int16);
	virtual bool RepairerWillRepairQueued(const unsigned __int16, const unsigned __int16);
	virtual bool RepairerCantRepair(const unsigned __int16, const unsigned __int16);
	virtual bool BridgeLayerWillLayBridge(const unsigned __int16, float, MC_Vector3<float>);
	virtual bool BridgeLayerCantLayBridge(const unsigned __int16);
	virtual bool LayBridgeQueued(const unsigned __int16);
	virtual bool BridgeLayerWillRemoveBridge(const unsigned __int16);
	virtual bool BridgeLayerCantRemoveBridge(const unsigned __int16);
	virtual bool BridgeLayerIsBridge(const unsigned __int16);
	virtual bool UnitSetFireBehaviour(const unsigned __int16, EXCO_Behaviour::Fire);
	virtual bool UnitSetMaxSpeedModifier(const unsigned __int16, const float);
	virtual bool SupportThingUsed(const unsigned int, MC_Vector3<float>, const bool);
	virtual bool SupportThingReady(const unsigned int);
	virtual bool SupportThingNotUsed(const unsigned int);
	virtual bool SupportThingSpawned(const unsigned int, MC_Vector3<float>, const int, unsigned int, MC_Vector3<float>, bool);
	virtual bool SupportThingSpawnedDelayed(const unsigned int, MC_Vector3<float>, const int, unsigned int, MC_Vector3<float>, float, const char *);
	virtual bool SupportThingCanceled(const char *);
	virtual bool SupportThingFeedback(const unsigned int, const int);
	virtual bool SupportThingMarker(const unsigned int, const unsigned int, MC_Vector3<float>, const int, unsigned int, MC_Vector3<float>, float);
	virtual bool SupportThingMarkerStopped(const unsigned int);
	virtual bool SupportThingSuccess(const unsigned int, EXCO_SupportThing::SupportThingSuccess);
	virtual bool SupportThingRecharge(const unsigned int, const float);
	virtual bool ResupplyBegin(const char, const unsigned __int16, MC_Vector3<float>);
	virtual bool ResupplyReady(const unsigned __int16);
	virtual bool ResupplyCanceled(const char, const unsigned __int16);
	virtual bool ResupplyFinished(const unsigned __int16);
	virtual bool MapSignal(const char, MC_Vector3<float> *);
	virtual bool SetWeatherEffect(const char *);
	virtual bool ShowSystemMessage(EXCO_ErrorMessage::Error, const int, const bool, const wchar_t *);
	virtual bool ShowMessageBox(const int, const int, const int, MC_Vector3<float> *, bool);
	virtual bool HideMessageBox(float);
	virtual bool PurgeMessageBoxQueue();
	virtual bool ShowPlayerGiveTANotification(const char, const char, const unsigned __int16);
	virtual bool ShowTimedMessageBox2(const char *, float, int);
	virtual bool ShowAutomaticMessageBox2(const char *, int);
	virtual bool ShowButtonMessageBox2(const char *, int);
	virtual bool PurgeMessageBoxQueue2();
	virtual bool FlashWidget(const char *, float, unsigned int);
	virtual bool SetWidgetStatus(const char *, int);
	virtual bool ClientPythonCommand(const char *, void *);
	virtual bool UpdateBalanceFactor(float);
	virtual bool UpdateTickSpeed(const int);
	virtual bool AddAIUnitObjective(unsigned __int16, int);
	virtual bool CreateCloud(EXCO_CloudType *, MC_Vector3<float> *, const float, const float, const int);
	virtual bool TeamWins(const int, EXCO_Constants::WinType);
	virtual bool MassgateStatsLogged();
	virtual bool SetScore(const char, const int);
	virtual bool SetScoreAtGameEnd(const char, const int, const int, const unsigned int, const unsigned int, const unsigned int, const unsigned int, const unsigned int, const unsigned int, const unsigned int, const unsigned int, const unsigned int, const unsigned int, const unsigned int);
	virtual bool SetExperienceLevel(unsigned __int16, char);
	virtual bool SpawnerAvailable(bool);
	virtual bool SpawnerDeployed(const char, const unsigned int, MC_Vector3<float> *, MC_Vector3<float> *, MC_GrowingArray<unsigned int> *, float, bool);
	virtual bool SpawnerSetPosition(const char, MC_Vector3<float> *);
	virtual bool RemoveAllSpawners();
	virtual bool SpawnerSpawning();
	virtual bool SpawnerSpawningDone(MC_GrowingArray<int> *);
	virtual bool SetGameModeData(WICO_GameMode::GameModeDataType, int);
	virtual bool SetGameModeData(WICO_GameMode::GameModeDataType, float);
	virtual bool SetUnitPrisoner(const int, const bool);
	virtual bool StartGameLogic();
	virtual bool StartGameTime();
	virtual bool AddDeploymentZone(const char *);
	virtual bool RemoveDeploymentZone(const char *);
	virtual bool AddDeploymentZoneToTeam(const char *, const int);
	virtual bool RemoveDeploymentZoneFromTeam(const char *, const int);
	virtual bool SetPauseState(const bool);
	virtual const char *GetStatus();
	virtual bool CreateUnitRelation(EXCO_Relation::Unit::Type, const unsigned __int16, const unsigned __int16);
	virtual bool DestroyUnitRelations(const unsigned __int16);
	virtual bool DestroyUnitRelations(EXCO_Relation::Unit::Type, const unsigned __int16, const unsigned __int16);
	virtual bool CreateBuildingRelation(EXCO_Relation::Building::Type, const unsigned __int16, const int);
	virtual bool DestroyBuildingRelations(const unsigned __int16);
	virtual bool DestroyBuildingRelations(EXCO_Relation::Building::Type, const unsigned __int16, const int);
	virtual bool RemotePlayerReady();
	virtual void HostShuttingDown();
	virtual void ShutdownImminentWarning(const unsigned int);
	virtual void ReceiveAIMsgCPMissionAquired(const int);
	virtual void ReceiveAIMsgCPMissionEnded(const int);
	virtual void ReceiveAIMsgAirEscortNeed(const int);
	virtual void ReceiveAIMsgAirEscortMissonAquired(const int);
	virtual void ReceiveAIMsgAirEscortReceived(const int);
	virtual void ReceiveAIMsgHealingNeed(const int);
	virtual void ReceiveAIMsgHealingMissionAquired(const int);
	virtual void ReceiveAIMsgHealingReceived(const int);
	virtual void ReceiveAIMsgFireSuppotNeed(MC_Vector3<float>);
	virtual void ReceiveAIMsgFireSuppotMissionAquired(MC_Vector3<float>);
	virtual void ReceiveAIMsgFireSuppotReceived(MC_Vector3<float>);
	virtual void ReceiveAIMsgBackupNeed(const int);
	virtual void ReceiveAIMsgBackupMissionAquired(const int);
	virtual void ReceiveAIMsgBackupReceived(const int);
	virtual void ReceiveAIMsgTANeed(const int);
	virtual void ReceiveAIMsgTAMissionAquired(const int);
	virtual void ReceiveAIMsgTAReceived(const int);
	virtual void SpecialDebugPrintOutput();
	virtual void SpeedTreeStateChange(WICO_SpeedTree::WICO_SpeedTreeNetData *);
	virtual void NukeGroundEffectBeforeJoin(WICO_NukeGroundEffect *);
	virtual void SetMRBState(const char *, const char *);
	virtual void ShowMinimapIcon(const char *, MC_Vector3<float> *, float, float, float, float, float);
	virtual bool SetCameraFromScript(MC_Vector3<float> *, MC_Vector3<float> *);
	virtual bool CameraSplineFromFileFromScript(const char *, int, float, float, MC_Vector3<float> *, MC_Vector3<float> *);
	virtual bool SetObjective(unsigned int, unsigned int, bool, int, MC_Vector3<float> *, EXCO_Constants::ObjectiveState, EXCO_Constants::ObjectiveType);
	virtual bool UpdateObjective(unsigned int, unsigned int, unsigned int, bool, int, MC_Vector3<float> *, EXCO_Constants::ObjectiveType);
	virtual int AddSubObjectiveMarker(unsigned int, const int, MC_Vector3<float> *);
	virtual bool RemoveSubObjectiveMarker(unsigned int, int, const bool);
	virtual bool ShowTimer(unsigned int, const char *, float);
	virtual bool AddSupportWeapon(unsigned int);
	virtual bool RemoveSupportWeapon(unsigned int);
	virtual bool ClearSupportWeapons();
	virtual void CutPathFindingHull(WICO_Hull *, WICAI_PathType);
	virtual void AddPathfindingHull(WICO_Hull *, WICAI_PathType);
	virtual void BurnForest(MC_Vector3<float>, float);
	virtual bool IsRemote();
	virtual void SetDestructionCell(const int, const float);
	virtual bool ClearMapCycle();
	virtual bool AddToMapCycle(__int64);
	virtual bool RemoveAllWarfilters();
	virtual void SetCoreSystemState(EXCO_Constants::CoreSystem, int);
	virtual void FadeIn(float);
	virtual void EnableSpawner(bool);
	virtual bool AddScriptEvent(int, int);
	virtual void RequestSent(WICO_Request::RequestType, unsigned int, char, float, int, MC_Vector2<float> *, unsigned int, const wchar_t *);
	virtual void RequestAccepted(unsigned int, char);
	virtual void DisableAllUnits(bool);
	virtual void RefreshReinforcementGuiUnits();
	virtual void SetPlayerLANName(const char, const wchar_t *);
	virtual void SendCameraPosition(MC_Vector3<float>);
	virtual void SendCameraRotation(MC_Vector2<float>);
	virtual void SendCameraHeightOverGround(const float);
	virtual bool SetShooterNumRounds(unsigned __int16, unsigned __int16, unsigned __int16);
	virtual bool SetShooterAmmoType(unsigned __int16, unsigned __int16, unsigned int);
	virtual void PropagatePCC(MMG_PCCProtocol::MassgateToClientGetPCC::PCCResponse *);
	virtual bool SaveClientStateForUnit(unsigned __int16);
	virtual bool RestoreClientStateForUnit(unsigned __int16, unsigned __int16);
	virtual bool BotModeChanged(unsigned int);
	virtual bool SetServerAdmin(char, bool);
	virtual void SetPresetTeams(WICO_PresetTeamInfo *);
	virtual void UpdateRankBalanceValues(MC_GrowingArray<float> *);
	virtual void UpdateRankBalanceInfo(EXCO_MissionInfo::RankBalanceInfo *);
	virtual bool BindGenericModelSoundToBone(const int, char *, char *);
	virtual void EnableDisbandUnit(bool);
	virtual void EnableHoldFire(bool);
	virtual void SetSPEndBlackBackground(bool);
	virtual void ForceNewDeploymentPosition();*/
};
#pragma pack(pop)