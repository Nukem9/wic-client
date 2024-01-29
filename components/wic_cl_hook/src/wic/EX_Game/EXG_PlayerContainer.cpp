#include "EXG_Player.h"
#include "EXG_PlayerContainer.h"

EXCO_IPlayer *EXG_PlayerContainer::GetPlayerForSlot(u8 aSlot) const
{
    if (aSlot >= EX_MAX_NUM_CLIENTS)
        return nullptr;

    if (!myPlayers[aSlot] || myPlayers[aSlot]->myPlayerDeletedFlag)
        return nullptr;

    return &myPlayers[aSlot]->myPlayer;
}

EXG_Player *EXG_PlayerContainer::GetGamePlayerForSlot(u8 aSlot) const
{
    if (aSlot == 255)
        return myScriptGamePlayer;

    if (aSlot >= EX_MAX_NUM_CLIENTS)
        return nullptr;

    return myPlayers[aSlot];
}
