/*
 * This file is part of the AzerothCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BATTLEFIELD_WG_
#define BATTLEFIELD_WG_

#include "Battlefield.h"
#include "Log.h"
#include "World.h"
#include "WorldState.h"
#include "WorldStateDefines.h"
#include "WorldStatePackets.h"

class Group;
class BattlefieldWG;
class WintergraspCapturePoint;

struct BfWGGameObjectBuilding;
struct WGWorkshop;

using GameObjectSet = std::set<GameObject*>;
using GameObjectBuilding = std::set<BfWGGameObjectBuilding*>;
using Workshop = std::set<WGWorkshop*>;
using GroupSet = std::set<Group*>;
//typedef std::set<WintergraspCapturePoint *> CapturePointSet; unused ?

uint32 const VehNumWorldState[2] = { WORLD_STATE_BATTLEFIELD_WG_VEHICLE_A, WORLD_STATE_BATTLEFIELD_WG_VEHICLE_H };
uint32 const MaxVehNumWorldState[2] = { WORLD_STATE_BATTLEFIELD_WG_MAX_VEHICLE_A, WORLD_STATE_BATTLEFIELD_WG_MAX_VEHICLE_H };
uint32 const ClockWorldState[2] = { WORLD_STATE_BATTLEFIELD_WG_CLOCK, WORLD_STATE_BATTLEFIELD_WG_CLOCK_TEXTS };
uint32 const WintergraspFaction[3] = { 1, 2, 35 };
float const WintergraspStalkerPos[4]    = { 4948.985f, 2937.789f, 550.5172f, 1.815142f };

enum WintergraspSpells
{
    // Wartime auras
    SPELL_RECRUIT                               = 37795,
    SPELL_CORPORAL                              = 33280,
    SPELL_LIEUTENANT                            = 55629,
    SPELL_TENACITY                              = 58549,
    SPELL_TENACITY_VEHICLE                      = 59911,
    SPELL_TOWER_CONTROL                         = 62064,
    SPELL_SPIRITUAL_IMMUNITY                    = 58729,
    SPELL_GREAT_HONOR                           = 58555,
    SPELL_GREATER_HONOR                         = 58556,
    SPELL_GREATEST_HONOR                        = 58557,
    SPELL_ALLIANCE_FLAG                         = 14268,
    SPELL_HORDE_FLAG                            = 14267,
    SPELL_GRAB_PASSENGER                        = 61178,

    // Reward spells
    SPELL_VICTORY_REWARD                        = 56902,
    SPELL_DEFEAT_REWARD                         = 58494,
    SPELL_DAMAGED_TOWER                         = 59135,
    SPELL_DESTROYED_TOWER                       = 59136,
    SPELL_DAMAGED_BUILDING                      = 59201,
    SPELL_INTACT_BUILDING                       = 59203,

    SPELL_TELEPORT_BRIDGE                       = 59096,
    SPELL_TELEPORT_FORTRESS                     = 60035,

    SPELL_TELEPORT_DALARAN                      = 53360,
    SPELL_VICTORY_AURA                          = 60044,

    // Other spells
    SPELL_WINTERGRASP_WATER                     = 36444,
    SPELL_ESSENCE_OF_WINTERGRASP                = 58045,
    SPELL_WINTERGRASP_RESTRICTED_FLIGHT_AREA    = 58730,
    SPELL_LEANING_TOWER_ACHIEVEMENT             = 60676,

    // Phasing spells
    SPELL_HORDE_CONTROLS_FACTORY_PHASE_SHIFT    = 56618,// ADDS PHASE 16
    SPELL_ALLIANCE_CONTROLS_FACTORY_PHASE_SHIFT = 56617,// ADDS PHASE 32

    SPELL_HORDE_CONTROL_PHASE_SHIFT             = 55773,// ADDS PHASE 64
    SPELL_ALLIANCE_CONTROL_PHASE_SHIFT          = 55774,// ADDS PHASE 128
};

enum WintergraspData
{
    BATTLEFIELD_WG_DATA_INTACT_TOWER_ATT,
    BATTLEFIELD_WG_DATA_DAMAGED_TOWER_ATT,
    BATTLEFIELD_WG_DATA_BROKEN_TOWER_ATT,
    BATTLEFIELD_WG_DATA_MAX_VEHICLE_A,
    BATTLEFIELD_WG_DATA_MAX_VEHICLE_H,
    BATTLEFIELD_WG_DATA_VEHICLE_A,
    BATTLEFIELD_WG_DATA_VEHICLE_H,
    BATTLEFIELD_WG_DATA_MAX,
};

/*#########################
 *####### Graveyards ######
 *#########################*/

class BfGraveyardWG : public BfGraveyard
{
public:
    BfGraveyardWG(BattlefieldWG* Bf);

    void SetTextId(uint32 textid) { m_GossipTextId = textid; }
    uint32 GetTextId() { return m_GossipTextId; }
protected:
    uint32 m_GossipTextId;
};

enum WGGraveyardId
{
    BATTLEFIELD_WG_GY_WORKSHOP_NE,
    BATTLEFIELD_WG_GY_WORKSHOP_NW,
    BATTLEFIELD_WG_GY_WORKSHOP_SE,
    BATTLEFIELD_WG_GY_WORKSHOP_SW,
    BATTLEFIELD_WG_GY_KEEP,
    BATTLEFIELD_WG_GY_HORDE,
    BATTLEFIELD_WG_GY_ALLIANCE,
    BATTLEFIELD_WG_GRAVEYARD_MAX,
};

enum eWGGossipText
{
    BATTLEFIELD_WG_GOSSIPTEXT_GY_NE              = 20071,
    BATTLEFIELD_WG_GOSSIPTEXT_GY_NW              = 20072,
    BATTLEFIELD_WG_GOSSIPTEXT_GY_SE              = 20074,
    BATTLEFIELD_WG_GOSSIPTEXT_GY_SW              = 20073,
    BATTLEFIELD_WG_GOSSIPTEXT_GY_KEEP            = 20070,
    BATTLEFIELD_WG_GOSSIPTEXT_GY_HORDE           = 20075,
    BATTLEFIELD_WG_GOSSIPTEXT_GY_ALLIANCE        = 20076,
    BATTLEFIELD_WG_WORLD_START_MESSAGE           = 20078,
};

enum WintergraspNpcs
{
    BATTLEFIELD_WG_NPC_GUARD_H                      = 30739,
    BATTLEFIELD_WG_NPC_GUARD_A                      = 30740,
    BATTLEFIELD_WG_NPC_STALKER                      = 15214,

    BATTLEFIELD_WG_NPC_VIERON_BLAZEFEATHER          = 31102,
    BATTLEFIELD_WG_NPC_STONE_GUARD_MUKAR            = 32296,// <WINTERGRASP QUARTERMASTER>
    BATTLEFIELD_WG_NPC_HOODOO_MASTER_FU_JIN         = 31101,// <MASTER HEXXER>
    BATTLEFIELD_WG_NPC_CHAMPION_ROS_SLAI            = 39173,// <WINTERGRASP QUARTERMASTER>
    BATTLEFIELD_WG_NPC_COMMANDER_DARDOSH            = 31091,
    BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_KILRATH     = 31151,
    BATTLEFIELD_WG_NPC_SIEGESMITH_STRONGHOOF        = 31106,
    BATTLEFIELD_WG_NPC_PRIMALIST_MULFORT            = 31053,
    BATTLEFIELD_WG_NPC_LIEUTENANT_MURP              = 31107,
    BATTLEFIELD_WG_NPC_HORDE_WARBRINGER             = 15350,
    BATTLEFIELD_WG_NPC_BRIGADIER_GENERAL            = 15351,

    BATTLEFIELD_WG_NPC_AV_PORTAL_HORDE              = 32617,
    BATTLEFIELD_WG_NPC_AV_PORTAL_ALLY               = 32616,
    BATTLEFIELD_WG_NPC_AB_PORTAL_HORDE              = 32619,
    BATTLEFIELD_WG_NPC_AB_PORTAL_ALLY               = 32618,
    BATTLEFIELD_WG_NPC_EOTS_PORTAL_HORDE            = 32620,
    BATTLEFIELD_WG_NPC_EOTS_PORTAL_ALLY             = 32621,
    BATTLEFIELD_WG_NPC_SOTA_PORTAL_HORDE            = 32623,
    BATTLEFIELD_WG_NPC_SOTA_PORTAL_ALLY             = 32622,
    BATTLEFIELD_WG_NPC_WG_PORTAL_HORDE              = 32625,
    BATTLEFIELD_WG_NPC_WG_PORTAL_ALLY               = 32624,

    BATTLEFIELD_WG_NPC_BOWYER_RANDOLPH              = 31052,
    BATTLEFIELD_WG_NPC_KNIGHT_DAMERON               = 32294,// <WINTERGRASP QUARTERMASTER>
    BATTLEFIELD_WG_NPC_SORCERESS_KAYLANA            = 31051,// <ENCHANTRESS>
    BATTLEFIELD_WG_NPC_MARSHAL_MAGRUDER             = 39172,// <WINTERGRASP QUARTERMASTER>
    BATTLEFIELD_WG_NPC_COMMANDER_ZANNETH            = 31036,
    BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_AHBRAMIS    = 31153,
    BATTLEFIELD_WG_NPC_SIEGE_MASTER_STOUTHANDLE     = 31108,
    BATTLEFIELD_WG_NPC_ANCHORITE_TESSA              = 31054,
    BATTLEFIELD_WG_NPC_SENIOR_DEMOLITIONIST_LEGOSO  = 31109,

    NPC_TAUNKA_SPIRIT_GUIDE                         = 31841,    // Horde spirit guide for Wintergrasp
    NPC_DWARVEN_SPIRIT_GUIDE                        = 31842,    // Alliance spirit guide for Wintergrasp

    NPC_WINTERGRASP_SIEGE_ENGINE_TURRET_ALLIANCE    = 28319,
    NPC_WINTERGRASP_SIEGE_ENGINE_TURRET_HORDE       = 32629,
    NPC_WINTERGRASP_SIEGE_ENGINE_ALLIANCE           = 28312,
    NPC_WINTERGRASP_SIEGE_ENGINE_HORDE              = 32627,
    NPC_WINTERGRASP_CATAPULT                        = 27881,
    NPC_WINTERGRASP_DEMOLISHER                      = 28094,
    NPC_WINTERGRASP_TOWER_CANNON                    = 28366,

    NPC_QUEST_SOUTHERN_TOWER_KILL                   = 35074,
    NPC_QUEST_VEHICLE_PROTECTED                     = 31284,
    NPC_QUEST_PVP_KILL_VEHICLE                      = 31093,
    NPC_QUEST_PVP_KILL_HORDE                        = 39019,
    NPC_QUEST_PVP_KILL_ALLIANCE                     = 31086,
};

struct BfWGCoordGY
{
    float x;
    float y;
    float z;
    float o;
    uint32 gyid;
    uint8 type;
    uint32 textid;              // for gossip menu
    TeamId startcontrol;
};

uint32 const WGQuest[2][6] =
{
    { 13186, 13181, 13222, 13538, 13177, 13179 },
    { 13185, 13183, 13223, 13539, 13178, 13180 },
};

// 7 in sql, 7 in header
BfWGCoordGY const WGGraveyard[BATTLEFIELD_WG_GRAVEYARD_MAX] =
{
    { 5104.750f, 2300.940f, 368.579f, 0.733038f, 1329, BATTLEFIELD_WG_GY_WORKSHOP_NE, BATTLEFIELD_WG_GOSSIPTEXT_GY_NE, TEAM_NEUTRAL },
    { 5099.120f, 3466.036f, 368.484f, 5.317802f, 1330, BATTLEFIELD_WG_GY_WORKSHOP_NW, BATTLEFIELD_WG_GOSSIPTEXT_GY_NW, TEAM_NEUTRAL },
    { 4314.648f, 2408.522f, 392.642f, 6.268125f, 1333, BATTLEFIELD_WG_GY_WORKSHOP_SE, BATTLEFIELD_WG_GOSSIPTEXT_GY_SE, TEAM_NEUTRAL },
    { 4331.716f, 3235.695f, 390.251f, 0.008500f, 1334, BATTLEFIELD_WG_GY_WORKSHOP_SW, BATTLEFIELD_WG_GOSSIPTEXT_GY_SW, TEAM_NEUTRAL },
    { 5537.986f, 2897.493f, 517.057f, 4.819249f, 1285, BATTLEFIELD_WG_GY_KEEP, BATTLEFIELD_WG_GOSSIPTEXT_GY_KEEP, TEAM_NEUTRAL },
    { 5032.454f, 3711.382f, 372.468f, 3.971623f, 1331, BATTLEFIELD_WG_GY_HORDE, BATTLEFIELD_WG_GOSSIPTEXT_GY_HORDE, TEAM_HORDE },
    { 5140.790f, 2179.120f, 390.950f, 1.972220f, 1332, BATTLEFIELD_WG_GY_ALLIANCE, BATTLEFIELD_WG_GOSSIPTEXT_GY_ALLIANCE, TEAM_ALLIANCE },
};

/* ######################### *
 *  WintergraspCapturePoint  *
 * ######################### */

class WintergraspCapturePoint : public BfCapturePoint
{
public:
    WintergraspCapturePoint(BattlefieldWG* battlefield, TeamId teamInControl);

    void LinkToWorkshop(WGWorkshop* workshop) { m_Workshop = workshop; }

    void ChangeTeam(TeamId oldteam) override;
    TeamId GetTeam() const { return m_team; }

protected:
    WGWorkshop* m_Workshop;
};

/* ######################### *
 * WinterGrasp Battlefield   *
 * ######################### */

class BattlefieldWG : public Battlefield
{
public:
    ~BattlefieldWG() override;
    /**
     * \brief Called when the battle start
     * - Spawn relic and turret
     * - Rebuild tower and wall
     * - Invite player to war
     */
    void OnBattleStart() override;

    /**
     * \brief Called when battle end
     * - Remove relic and turret
     * - Change banner/npc in keep if it needed
     * - Saving battlestate
     * - Reward honor/mark to player
     * - Remove vehicle
     * \param endByTimer : true if battle ended when timer is at 00:00, false if battle ended by clicking on relic
     */
    void OnBattleEnd(bool endByTimer) override;

    /**
     * \brief Called when grouping starts (15 minutes before battlestart)
     * - Invite all player in zone to join queue
     */
    void OnStartGrouping() override;

    /**
     * \brief Called when player accept invite to join battle
     * - Update aura
     * - Teleport if it needed
     * - Update worldstate
     * - Update tenacity
     * \param player: Player who accepted invite
     */
    void OnPlayerJoinWar(Player* player) override;

    /**
     * \brief Called when player left the battle
     * - Update player aura
     * \param player : Player who left the battle
     */
    void OnPlayerLeaveWar(Player* player) override;

    /**
     * \brief Called when player left the WG zone
     * \param player : Player who left the zone
     */
    void OnPlayerLeaveZone(Player* player) override;

    /**
     * \brief Called when player enters in WG zone
     * - Update aura
     * - Update worldstate
     * \param player : Player who enters the zone
     */
    void OnPlayerEnterZone(Player* player) override;

    /**
     * \brief Called for update battlefield data
     * - Save battle timer in database every minutes
     * - Update imunity aura from graveyard
     * \param diff : time elapsed since the last call (in ms)
     */
    bool Update(uint32 diff) override;

    /**
     * \brief Called when a creature is created
     * - Update vehicle count
     */
    void OnCreatureCreate(Creature* creature) override;

    /**
     * \brief Called when a creature is removed
     * - Update vehicle count
     */
    void OnCreatureRemove(Creature* creature) override;

    /**
     * \brief Called when a gameobject is created
     */
    void OnGameObjectCreate(GameObject* go) override;

    /**
     * \brief Called when a wall/tower is broken
     * - Update quest
     */
    void BrokenWallOrTower(TeamId team);

    /**
     * \brief Called when a tower is damaged
     * - Update tower count (for reward calcul)
     */
    void UpdateDamagedTowerCount(TeamId team);

    /**
     * \brief Called when tower is broken
     * - Update tower buff
     * - check if three south tower is down for remove 10 minutes to wg
     */
    void UpdatedDestroyedTowerCount(TeamId team, GameObject* go);

    //void DoCompleteOrIncrementAchievement(uint32 achievement, Player* player, uint8 incrementNumber = 1);

    void RemoveAurasFromPlayer(Player* player);

    /**
     * \brief Called when battlefield is setup, at server start
     */
    bool SetupBattlefield() override;

    /// Return pointer to relic object
    GameObject* GetRelic() { return GetGameObject(m_titansRelic); }

    /// Define relic object
    //void SetRelic(GameObject* relic) { m_titansRelic = relic; }

    /// Check if players can interact with the relic (Only if the last door has been broken)
    bool CanInteractWithRelic() { return m_isRelicInteractible; }

    /// Define if player can interact with the relic
    void SetRelicInteractible(bool allow) { m_isRelicInteractible = allow; }

    /// Vehicle world states update
    void UpdateCounterVehicle(bool init);
    void UpdateVehicleCountWG();
    void CapturePointTaken(uint32 areaId);

    void SendInitWorldStatesTo(Player* player);
    void SendInitWorldStatesToAll() override;
    void FillInitialWorldStates(WorldPackets::WorldState::InitWorldStates& packet) override;
    void SendUpdateWorldStates(Player* player = nullptr) override;
    void SendUpdateWorldStateMessage(uint32 variable, uint32 value, Player* player = nullptr);

    void HandleKill(Player* killer, Unit* victim) override;
    void OnUnitDeath(Unit* unit) override;
    void PromotePlayer(Player* killer);

    uint32 GetHonorBuff(int32 stack) const;
    void UpdateTenacity();
    void AddUpdateTenacity(Player* player);
    void RemoveUpdateTenacity(Player* player);
    void ProcessEvent(WorldObject* obj, uint32 eventId) override;

    bool FindAndRemoveVehicleFromList(Unit* vehicle);

    // returns the graveyardId in the specified area.
    uint8 GetSpiritGraveyardId(uint32 areaId) const;
    uint32 GetAreaByGraveyardId(uint8 gId) const;

    uint32 GetData(uint32 data) const override;

    bool IsKeepNpc(uint32 entry)
    {
        switch (entry)
        {
            case BATTLEFIELD_WG_NPC_GUARD_H:
            case BATTLEFIELD_WG_NPC_GUARD_A:
            case BATTLEFIELD_WG_NPC_VIERON_BLAZEFEATHER:
            case BATTLEFIELD_WG_NPC_BOWYER_RANDOLPH:
            case BATTLEFIELD_WG_NPC_STONE_GUARD_MUKAR:
            case BATTLEFIELD_WG_NPC_KNIGHT_DAMERON:
            case BATTLEFIELD_WG_NPC_HOODOO_MASTER_FU_JIN:
            case BATTLEFIELD_WG_NPC_SORCERESS_KAYLANA:
            case BATTLEFIELD_WG_NPC_CHAMPION_ROS_SLAI:
            case BATTLEFIELD_WG_NPC_MARSHAL_MAGRUDER:
            case BATTLEFIELD_WG_NPC_COMMANDER_DARDOSH:
            case BATTLEFIELD_WG_NPC_COMMANDER_ZANNETH:
            case BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_KILRATH:
            case BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_AHBRAMIS:
            case BATTLEFIELD_WG_NPC_HORDE_WARBRINGER:
            case BATTLEFIELD_WG_NPC_BRIGADIER_GENERAL:
            case BATTLEFIELD_WG_NPC_SIEGESMITH_STRONGHOOF:
            case BATTLEFIELD_WG_NPC_SIEGE_MASTER_STOUTHANDLE:
            case BATTLEFIELD_WG_NPC_PRIMALIST_MULFORT:
            case BATTLEFIELD_WG_NPC_ANCHORITE_TESSA:
            case BATTLEFIELD_WG_NPC_LIEUTENANT_MURP:
            case BATTLEFIELD_WG_NPC_SENIOR_DEMOLITIONIST_LEGOSO:
                return true;
        }
        return false;
    }
protected:
    bool m_isRelicInteractible;

    Workshop WorkshopsList;

    GameObjectSet DefenderPortalList;
    GameObjectSet m_KeepGameObject[2];
    GameObjectBuilding BuildingsInZone;

    GuidUnorderedSet m_vehicles[2];
    GuidUnorderedSet CanonList;
    GuidUnorderedSet KeepCreature[2];
    GuidUnorderedSet OutsideCreature[2];
    GuidUnorderedSet m_updateTenacityList;

    int32 m_tenacityStack;
    uint32 m_tenacityUpdateTimer;
    uint32 m_saveTimer;

    ObjectGuid m_titansRelic;
};

uint8 const WG_MAX_OBJ = 32;
uint8 const WG_MAX_TURRET = 16;
uint8 const WG_MAX_KEEP_NPC = 45;
uint8 const WG_MAX_OUTSIDE_NPC = 14;
uint8 const WG_OUTSIDE_ALLIANCE_NPC = 7;
uint8 const WG_MAX_TELEPORTER = 12;

enum WintergraspGameObjectBuildingType
{
    BATTLEFIELD_WG_OBJECTTYPE_DOOR,
    BATTLEFIELD_WG_OBJECTTYPE_TITANRELIC,
    BATTLEFIELD_WG_OBJECTTYPE_WALL,
    BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST,
    BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER,
    BATTLEFIELD_WG_OBJECTTYPE_TOWER,
};

enum WintergraspGameObjectState
{
    BATTLEFIELD_WG_OBJECTSTATE_NONE,
    BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_INTACT,
    BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_DAMAGE,
    BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_DESTROY,
    BATTLEFIELD_WG_OBJECTSTATE_HORDE_INTACT,
    BATTLEFIELD_WG_OBJECTSTATE_HORDE_DAMAGE,
    BATTLEFIELD_WG_OBJECTSTATE_HORDE_DESTROY,
    BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT,
    BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DAMAGE,
    BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DESTROY,
};

enum WintergraspWorkshopIds
{
    BATTLEFIELD_WG_WORKSHOP_NE,
    BATTLEFIELD_WG_WORKSHOP_NW,
    BATTLEFIELD_WG_WORKSHOP_SE,
    BATTLEFIELD_WG_WORKSHOP_SW,
    BATTLEFIELD_WG_WORKSHOP_KEEP_WEST,
    BATTLEFIELD_WG_WORKSHOP_KEEP_EAST,
};

/// @todo: Handle this with creature_text ?
enum eWGText
{
    BATTLEFIELD_WG_TEXT_START                    = 28,
    BATTLEFIELD_WG_TEXT_WORKSHOP_NE_ATTACK       = 19, // Ally, horde +2
    BATTLEFIELD_WG_TEXT_WORKSHOP_NW_ATTACK       = 11, // Ally, horde +2
    BATTLEFIELD_WG_TEXT_WORKSHOP_SE_ATTACK       = 15, // Ally, horde +2
    BATTLEFIELD_WG_TEXT_WORKSHOP_SW_ATTACK       = 23, // Ally, horde +2
    BATTLEFIELD_WG_TEXT_WORKSHOP_NE_TAKEN        = 20, // Ally, horde +2
    BATTLEFIELD_WG_TEXT_WORKSHOP_NW_TAKEN        = 12, // Ally, horde +2
    BATTLEFIELD_WG_TEXT_WORKSHOP_SE_TAKEN        = 16, // Ally, horde +2
    BATTLEFIELD_WG_TEXT_WORKSHOP_SW_TAKEN        = 24, // Ally, horde +2
    BATTLEFIELD_WG_TEXT_DEFEND_KEEP              = 29, // Ally, horde +2
    BATTLEFIELD_WG_TEXT_WIN_KEEP                 = 30, // Ally, horde +2
    BATTLEFIELD_WG_TEXT_WILL_START               = 27,
    BATTLEFIELD_WG_TEXT_FIRSTRANK                = 37,
    BATTLEFIELD_WG_TEXT_SECONDRANK               = 38,
    BATTLEFIELD_WG_TEXT_TOWER_S_DAMAGED          = 1,
    BATTLEFIELD_WG_TEXT_TOWER_E_DAMAGED          = 3,
    BATTLEFIELD_WG_TEXT_TOWER_W_DAMAGED          = 5,
    BATTLEFIELD_WG_TEXT_TOWER_S_DESTROYED        = 2,
    BATTLEFIELD_WG_TEXT_TOWER_E_DESTROYED        = 4,
    BATTLEFIELD_WG_TEXT_TOWER_W_DESTROYED        = 6,
    BATTLEFIELD_WG_TEXT_KEEPTOWER_NE_DAMAGED     = 33,
    BATTLEFIELD_WG_TEXT_KEEPTOWER_NW_DAMAGED     = 7,
    BATTLEFIELD_WG_TEXT_KEEPTOWER_SE_DAMAGED     = 9,
    BATTLEFIELD_WG_TEXT_KEEPTOWER_SW_DAMAGED     = 35,
    BATTLEFIELD_WG_TEXT_KEEPTOWER_NE_DESTROYED   = 34,
    BATTLEFIELD_WG_TEXT_KEEPTOWER_NW_DESTROYED   = 8,
    BATTLEFIELD_WG_TEXT_KEEPTOWER_SE_DESTROYED   = 10,
    BATTLEFIELD_WG_TEXT_KEEPTOWER_SW_DESTROYED   = 36,
};

enum WintergraspGameObject
{
    GO_WINTERGRASP_FACTORY_BANNER_NE             = 190475,
    GO_WINTERGRASP_FACTORY_BANNER_NW             = 190487,
    GO_WINTERGRASP_FACTORY_BANNER_SE             = 194959,
    GO_WINTERGRASP_FACTORY_BANNER_SW             = 194962,

    GO_WINTERGRASP_TITAN_S_RELIC                 = 192829,

    GO_WINTERGRASP_FORTRESS_TOWER_1              = 190221,
    GO_WINTERGRASP_FORTRESS_TOWER_2              = 190373,
    GO_WINTERGRASP_FORTRESS_TOWER_3              = 190377,
    GO_WINTERGRASP_FORTRESS_TOWER_4              = 190378,

    GO_WINTERGRASP_SHADOWSIGHT_TOWER             = 190356,
    GO_WINTERGRASP_WINTER_S_EDGE_TOWER           = 190357,
    GO_WINTERGRASP_FLAMEWATCH_TOWER              = 190358,
};

struct WintergraspObjectPositionData
{
    float x;
    float y;
    float z;
    float o;
    uint32 entryHorde;
    uint32 entryAlliance;
};

// *****************************************************
// ************ Destructible (Wall,Tower..) ************
// *****************************************************

struct WintergraspBuildingSpawnData
{
    uint32 entry;
    uint32 WorldState;
    float x;
    float y;
    float z;
    float o;
    uint32 type;
    uint32 damageText;
    uint32 destroyText;
};

WintergraspBuildingSpawnData const WGGameObjectBuilding[WG_MAX_OBJ] =
{
    // Wall (Not spawned in db)
    // Entry  WS    X        Y        Z        O         type                          NameID
    { 190219, 3749, 5371.46f, 3047.47f, 407.571f, 3.14159f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 190220, 3750, 5331.26f, 3047.1f, 407.923f, 0.052359f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191795, 3764, 5385.84f, 2909.49f, 409.713f, 0.00872f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191796, 3772, 5384.45f, 2771.84f, 410.27f, 3.14159f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191799, 3762, 5371.44f, 2630.61f, 408.816f, 3.13286f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191800, 3766, 5301.84f, 2909.09f, 409.866f, 0.008724f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191801, 3770, 5301.06f, 2771.41f, 409.901f, 3.14159f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191802, 3751, 5280.2f, 2995.58f, 408.825f, 1.61443f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191803, 3752, 5279.14f, 2956.02f, 408.604f, 1.5708f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191804, 3767, 5278.69f, 2882.51f, 409.539f, 1.5708f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191806, 3769, 5279.5f, 2798.94f, 409.998f, 1.5708f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191807, 3759, 5279.94f, 2724.77f, 409.945f, 1.56207f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191808, 3760, 5279.6f, 2683.79f, 409.849f, 1.55334f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191809, 3761, 5330.96f, 2630.78f, 409.283f, 3.13286f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 190369, 3753, 5256.08f, 2933.96f, 409.357f, 3.13286f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 190370, 3758, 5257.46f, 2747.33f, 409.743f, -3.13286f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 190371, 3754, 5214.96f, 2934.09f, 409.19f, -0.008724f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 190372, 3757, 5215.82f, 2747.57f, 409.188f, -3.13286f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 190374, 3755, 5162.27f, 2883.04f, 410.256f, 1.57952f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 190376, 3756, 5163.72f, 2799.84f, 409.227f, 1.57952f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },

    // Tower of keep (Not spawned in db)
    { 190221, 3711, 5281.15f, 3044.59f, 407.843f, 3.11539f, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER, BATTLEFIELD_WG_TEXT_KEEPTOWER_NW_DAMAGED, BATTLEFIELD_WG_TEXT_KEEPTOWER_NW_DESTROYED },
    { 190373, 3713, 5163.76f, 2932.23f, 409.19f, 3.12412f, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER, BATTLEFIELD_WG_TEXT_KEEPTOWER_SW_DAMAGED, BATTLEFIELD_WG_TEXT_KEEPTOWER_SW_DESTROYED },
    { 190377, 3714, 5166.4f, 2748.37f, 409.188f, -1.5708f, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER, BATTLEFIELD_WG_TEXT_KEEPTOWER_SE_DAMAGED, BATTLEFIELD_WG_TEXT_KEEPTOWER_SE_DESTROYED },
    { 190378, 3712, 5281.19f, 2632.48f, 409.099f, -1.58825f, BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER, BATTLEFIELD_WG_TEXT_KEEPTOWER_NE_DAMAGED, BATTLEFIELD_WG_TEXT_KEEPTOWER_NE_DESTROYED },

    // Wall (with passage) (Not spawned in db)
    { 191797, 3765, 5343.29f, 2908.86f, 409.576f, 0.008724f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191798, 3771, 5342.72f, 2771.39f, 409.625f, 3.14159f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },
    { 191805, 3768, 5279.13f, 2840.8f, 409.783f, 1.57952f, BATTLEFIELD_WG_OBJECTTYPE_WALL, 0, 0 },

    // South tower (Not spawned in db)
    { 190356, 3704, 4557.17f, 3623.94f, 395.883f, 1.67552f, BATTLEFIELD_WG_OBJECTTYPE_TOWER, BATTLEFIELD_WG_TEXT_TOWER_W_DAMAGED, BATTLEFIELD_WG_TEXT_TOWER_W_DESTROYED },
    { 190357, 3705, 4398.17f, 2822.5f, 405.627f, -3.12412f, BATTLEFIELD_WG_OBJECTTYPE_TOWER, BATTLEFIELD_WG_TEXT_TOWER_S_DAMAGED, BATTLEFIELD_WG_TEXT_TOWER_S_DESTROYED },
    { 190358, 3706, 4459.1f, 1944.33f, 434.991f, -2.00276f, BATTLEFIELD_WG_OBJECTTYPE_TOWER, BATTLEFIELD_WG_TEXT_TOWER_E_DAMAGED, BATTLEFIELD_WG_TEXT_TOWER_E_DESTROYED },

    // Door of forteress (Not spawned in db)
    { 190375, 3763, 5162.99f, 2841.23f, 410.162f, -3.13286f, BATTLEFIELD_WG_OBJECTTYPE_DOOR, 0, 0 },

    // Last door (Not spawned in db)
    { 191810, 3773, 5397.11f, 2841.54f, 425.899f, 3.14159f, BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST, 0, 0 },
};

Position const WGTurret[WG_MAX_TURRET] =
{
    { 5391.08251953125f, 3061.720947265625f, 419.699462890625f, 1.570796370506286621f }, // VerifiedBuild 51666
    { 5265.90966796875f, 2976.45849609375f, 421.149261474609375f, 2.984513044357299804f }, // VerifiedBuild 51666
    { 5234.7861328125f, 2948.731689453125f, 420.96331787109375f, 1.623156189918518066f }, // VerifiedBuild 51666
    { 5322.8662109375f, 2923.34326171875f, 421.7288818359375f, 1.466076612472534179f }, // VerifiedBuild 51666
    { 5363.857421875f, 2923.782470703125f, 421.79150390625f, 1.48352980613708496f }, // VerifiedBuild 51666
    { 5264.23583984375f, 2861.381103515625f, 421.66876220703125f, 3.124139308929443359f }, // VerifiedBuild 51666
    { 5264.58544921875f, 2819.799560546875f, 421.738494873046875f, 3.106686115264892578f }, // VerifiedBuild 51666
    { 5322.18408203125f, 2756.658203125f, 421.72845458984375f, 4.520402908325195312f }, // VerifiedBuild 51666
    { 5363.7685546875f, 2756.81689453125f, 421.712127685546875f, 4.572762489318847656f }, // VerifiedBuild 51666
    { 5236.1044921875f, 2732.7265625f, 421.73175048828125f, 4.642575740814208984f }, // VerifiedBuild 51666
    { 5264.88720703125f, 2704.792236328125f, 421.782623291015625f, 3.211405754089355468f }, // VerifiedBuild 51666
    { 5350.78125f, 2615.8466796875f, 421.32568359375f, 4.747295379638671875f }, // VerifiedBuild 51666
    { 5390.77490234375f, 2615.3251953125f, 421.208984375f, 4.607669353485107421f }, // VerifiedBuild 51666
    { 5148.5634765625f, 2820.538330078125f, 421.70361328125f, 3.368485450744628906f }, // VerifiedBuild 51666
    { 5147.75f, 2861.8681640625f, 421.713165283203125f, 3.141592741012573242f }, // VerifiedBuild 51666
    { 5352.21923828125f, 3061.62158203125f, 421.184600830078125f, 1.675516128540039062f } // VerifiedBuild 51666
};

// Here there is all npc keeper spawn point
WintergraspObjectPositionData const WGKeepNPC[WG_MAX_KEEP_NPC] =
{
    // X          Y            Z           O         horde                          alliance
    // North East
    { 5326.203125f, 2660.026367f, 409.100891f, 2.543383f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Roaming Guard
    { 5298.430176f, 2738.760010f, 409.316010f, 3.971740f, BATTLEFIELD_WG_NPC_VIERON_BLAZEFEATHER, BATTLEFIELD_WG_NPC_BOWYER_RANDOLPH }, // Vieron Blazefeather
    { 5335.310059f, 2764.110107f, 409.274994f, 4.834560f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5349.810059f, 2763.629883f, 409.333008f, 4.660030f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    // North
    { 5373.470215f, 2789.060059f, 409.322998f, 2.600540f, BATTLEFIELD_WG_NPC_STONE_GUARD_MUKAR, BATTLEFIELD_WG_NPC_KNIGHT_DAMERON }, // Stone Guard Mukar
    { 5296.560059f, 2789.870117f, 409.274994f, 0.733038f, BATTLEFIELD_WG_NPC_HOODOO_MASTER_FU_JIN, BATTLEFIELD_WG_NPC_SORCERESS_KAYLANA }, // Voodoo Master Fu'jin
    { 5372.670000f, 2786.740000f, 409.442000f, 2.809980f, BATTLEFIELD_WG_NPC_CHAMPION_ROS_SLAI, BATTLEFIELD_WG_NPC_MARSHAL_MAGRUDER }, // Wintergrasp Quartermaster
    { 5368.709961f, 2856.360107f, 409.322998f, 2.949610f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5367.910156f, 2826.520020f, 409.322998f, 3.333580f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5389.270020f, 2847.370117f, 418.759003f, 3.106690f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5388.560059f, 2834.770020f, 418.759003f, 3.071780f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5359.129883f, 2837.989990f, 409.364014f, 4.698930f, BATTLEFIELD_WG_NPC_COMMANDER_DARDOSH, BATTLEFIELD_WG_NPC_COMMANDER_ZANNETH }, // Commander Dardosh
    { 5366.129883f, 2833.399902f, 409.322998f, 3.141590f, BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_KILRATH, BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_AHBRAMIS }, // Tactical Officer Kilrath
    { 5467.590000f, 2841.060000f, 418.758000f, 1.396260f, BATTLEFIELD_WG_NPC_HORDE_WARBRINGER, BATTLEFIELD_WG_NPC_BRIGADIER_GENERAL }, // Battlemaster

    { 5415.120000f, 2811.420000f, 418.758000f, 2.321290f, BATTLEFIELD_WG_NPC_AV_PORTAL_HORDE, BATTLEFIELD_WG_NPC_AV_PORTAL_ALLY }, // Portal
    { 5467.560000f, 2810.080000f, 418.758000f, 0.837758f, BATTLEFIELD_WG_NPC_AB_PORTAL_HORDE, BATTLEFIELD_WG_NPC_AB_PORTAL_ALLY }, // Portal
    { 5476.330000f, 2863.210000f, 418.758000f, 0.471239f, BATTLEFIELD_WG_NPC_EOTS_PORTAL_HORDE, BATTLEFIELD_WG_NPC_EOTS_PORTAL_ALLY }, // Portal
    { 5468.270000f, 2871.140000f, 418.758000f, 0.000000f, BATTLEFIELD_WG_NPC_SOTA_PORTAL_HORDE, BATTLEFIELD_WG_NPC_SOTA_PORTAL_ALLY }, // Portal
    { 5476.100000f, 2818.040000f, 418.758000f, 4.380780f, BATTLEFIELD_WG_NPC_WG_PORTAL_HORDE, BATTLEFIELD_WG_NPC_WG_PORTAL_ALLY }, // Portal

    // X          Y            Z           O         horde  alliance
    // North West
    { 5350.680176f, 2917.010010f, 409.274994f, 1.466080f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5335.120117f, 2916.800049f, 409.444000f, 1.500980f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5295.560059f, 2926.669922f, 409.274994f, 0.872665f, BATTLEFIELD_WG_NPC_SIEGESMITH_STRONGHOOF, BATTLEFIELD_WG_NPC_SIEGE_MASTER_STOUTHANDLE }, // Stronghoof
    { 5371.399902f, 3026.510010f, 409.205994f, 3.250030f, BATTLEFIELD_WG_NPC_PRIMALIST_MULFORT, BATTLEFIELD_WG_NPC_ANCHORITE_TESSA }, // Primalist Mulfort
    { 5392.123535f, 3031.110352f, 409.187683f, 3.677212f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Roaming Guard
    // South
    { 5270.060059f, 2847.550049f, 409.274994f, 3.071780f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5270.160156f, 2833.479980f, 409.274994f, 3.124140f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5179.109863f, 2837.129883f, 409.274994f, 3.211410f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5179.669922f, 2846.600098f, 409.274994f, 3.089230f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5234.970215f, 2883.399902f, 409.274994f, 4.293510f, BATTLEFIELD_WG_NPC_LIEUTENANT_MURP, BATTLEFIELD_WG_NPC_SENIOR_DEMOLITIONIST_LEGOSO }, // Lieutenant Murp
    // X          Y            Z           O         horde  alliance
    // Portal guards (from around the fortress)
    { 5319.209473f, 3055.947754f, 409.176636f, 1.020201f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5311.612305f, 3061.207275f, 408.734161f, 0.965223f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5264.713379f, 3017.283447f, 408.479706f, 3.482424f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5269.096191f, 3008.315918f, 408.826294f, 3.843706f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5201.414551f, 2945.096924f, 409.190735f, 0.945592f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5193.386230f, 2949.617188f, 409.190735f, 1.145859f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5148.116211f, 2904.761963f, 409.193756f, 3.368532f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5153.355957f, 2895.501465f, 409.199310f, 3.549174f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5154.353027f, 2787.349365f, 409.250183f, 2.555644f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5150.066406f, 2777.876953f, 409.343903f, 2.708797f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5193.706543f, 2732.882812f, 409.189514f, 4.845073f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5202.126953f, 2737.570557f, 409.189514f, 5.375215f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5269.181152f, 2671.174072f, 409.098999f, 2.457459f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5264.960938f, 2662.332520f, 409.098999f, 2.598828f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5307.111816f, 2616.006836f, 409.095734f, 5.355575f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },       // Standing Guard
    { 5316.770996f, 2619.430176f, 409.027740f, 5.363431f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A }        // Standing Guard
};

WintergraspObjectPositionData const WGOutsideNPC[WG_MAX_OUTSIDE_NPC] =
{
    { 5032.04f, 3681.79f, 362.980f, 4.210f, BATTLEFIELD_WG_NPC_VIERON_BLAZEFEATHER, 0 },
    { 5020.71f, 3626.19f, 360.150f, 4.640f, BATTLEFIELD_WG_NPC_HOODOO_MASTER_FU_JIN, 0 },
    { 4994.85f, 3660.51f, 359.150f, 2.260f, BATTLEFIELD_WG_NPC_COMMANDER_DARDOSH, 0 },
    { 5015.46f, 3677.11f, 362.970f, 6.009f, BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_KILRATH, 0 },
    { 5031.12f, 3663.77f, 363.500f, 3.110f, BATTLEFIELD_WG_NPC_SIEGESMITH_STRONGHOOF, 0 },
    { 5042.74f, 3675.82f, 363.060f, 3.358f, BATTLEFIELD_WG_NPC_PRIMALIST_MULFORT, 0 },
    { 5014.45f, 3640.87f, 361.390f, 3.280f, BATTLEFIELD_WG_NPC_LIEUTENANT_MURP, 0 },
    { 5100.07f, 2168.89f, 365.779f, 1.972f, 0, BATTLEFIELD_WG_NPC_BOWYER_RANDOLPH },
    { 5081.70f, 2173.73f, 365.878f, 0.855f, 0, BATTLEFIELD_WG_NPC_SORCERESS_KAYLANA },
    { 5078.28f, 2183.70f, 365.029f, 1.466f, 0, BATTLEFIELD_WG_NPC_COMMANDER_ZANNETH },
    { 5088.49f, 2188.18f, 365.647f, 5.253f, 0, BATTLEFIELD_WG_NPC_TACTICAL_OFFICER_AHBRAMIS },
    { 5095.67f, 2193.28f, 365.924f, 4.939f, 0, BATTLEFIELD_WG_NPC_SIEGE_MASTER_STOUTHANDLE },
    { 5088.61f, 2167.66f, 365.689f, 0.680f, 0, BATTLEFIELD_WG_NPC_ANCHORITE_TESSA },
    { 5080.40f, 2199.00f, 359.489f, 2.967f, 0, BATTLEFIELD_WG_NPC_SENIOR_DEMOLITIONIST_LEGOSO },
};

struct WintergraspTeleporterData
{
    uint32 entry;
    float x;
    float y;
    float z;
    float o;
};

WintergraspTeleporterData const WGPortalDefenderData[WG_MAX_TELEPORTER] =
{
    // Player teleporter
    { 190763, 5153.41f, 2901.35f, 409.191f, -0.069f },
    { 190763, 5268.70f, 2666.42f, 409.099f, -0.715f },
    { 190763, 5197.05f, 2944.81f, 409.191f, 2.3387f },
    { 190763, 5196.67f, 2737.34f, 409.189f, -2.932f },
    { 190763, 5314.58f, 3055.85f, 408.862f, 0.5410f },
    { 190763, 5391.28f, 2828.09f, 418.675f, -2.164f },
    { 190763, 5153.93f, 2781.67f, 409.246f, 1.6580f },
    { 190763, 5311.44f, 2618.93f, 409.092f, -2.373f },
    { 190763, 5269.21f, 3013.84f, 408.828f, -1.762f },
    { 190763, 5401.62f, 2853.66f, 418.674f, 2.6354f },
    // Vehicle teleporter
    { 192951, 5314.51f, 2703.69f, 408.550f, -0.890f },
    { 192951, 5316.25f, 2977.04f, 408.539f, -0.820f },
};

// *********************************************************
// **********Tower Element(GameObject,Creature)*************
// *********************************************************

struct WintergraspTowerData
{
    uint32 towerEntry;                  // Gameobject id of tower
    uint8 nbObject;                     // Number of gameobjects spawned on this point
    WintergraspObjectPositionData GameObject[6];   // Gameobject position and entry (Horde/Alliance)

    // Creature : Turrets and Guard, TODO: check if killed on tower destruction? tower damage?
    uint8 nbCreatureBottom;
    WintergraspObjectPositionData CreatureBottom[9];
    uint8 nbCreatureTop;
    WintergraspObjectPositionData CreatureTop[5];
};

uint8 const WG_MAX_ATTACKTOWERS = 3;

// 192414 : 0 in sql, 1 in header
// 192278 : 0 in sql, 3 in header
WintergraspTowerData const AttackTowers[WG_MAX_ATTACKTOWERS] =
{
    // West tower
    {
        190356,
        6,
        {
            { 4559.109863f, 3606.219971f, 419.998993f, -1.483530f, 192488, 192501 },    // Flag on tower
            { 4539.419922f, 3622.489990f, 420.033997f, -3.071770f, 192488, 192501 },    // Flag on tower
            { 4555.259766f, 3641.649902f, 419.973999f, 1.675510f, 192488, 192501 },     // Flag on tower
            { 4574.870117f, 3625.909912f, 420.079010f, 0.080117f, 192488, 192501 },     // Flag on tower
            { 4433.899902f, 3534.139893f, 360.274994f, -1.850050f, 192269, 192278 },    // Flag near workshop
            { 4572.930176f, 3475.520020f, 363.009003f, 1.42240f, 192269, 192278 }       // Flag near bridge
        },
        6,
        {
            { 4418.688477f, 3506.251709f, 358.975494f, 4.293305f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Roaming Guard
            { 4570.86f, 3581.62f, 393.77f, 4.76f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },
            { 4551.70f, 3579.83f, 394.21f, 4.80f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },
            { 4559.91f, 3536.51f, 380.17f, 4.66f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },
            { 4508.64f, 3608.25f, 392.50f, 3.27f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },
            { 4504.68f, 3629.90f, 393.19f, 3.71f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
        },
        0,
        {
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
        }
    },

    // South Tower
    {
        190357,
        5,
        {
            { 4416.000000f, 2822.669922f, 429.851013f, -0.017452f, 192488, 192501 },    // Flag on tower
            { 4398.819824f, 2804.699951f, 429.791992f, -1.588250f, 192488, 192501 },    // Flag on tower
            { 4387.620117f, 2719.570068f, 389.934998f, -1.544620f, 192366, 192414 },    // Flag near tower
            { 4464.120117f, 2855.449951f, 406.110992f, 0.829032f, 192366, 192429 },     // Flag near tower
            { 4526.459961f, 2810.179932f, 391.200012f, -2.993220f, 192269, 192278 },    // Flag near bridge
            { 0, 0, 0, 0, 0, 0 },
        },
        6,
        {
            { 4452.859863f, 2808.870117f, 402.604004f, 6.056290f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { 4455.899902f, 2835.958008f, 401.122559f, 0.034907f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { 4412.649414f, 2953.792236f, 374.799957f, 0.980838f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Roaming Guard
            { 4362.089844f, 2811.510010f, 407.337006f, 3.193950f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { 4412.290039f, 2753.790039f, 401.015015f, 5.829400f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { 4421.939941f, 2773.189941f, 400.894989f, 5.707230f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0},
            { 0, 0, 0, 0, 0, 0},
        },
        0,
        {
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
        },
    },

    // East Tower
    {
        190358,
        4,
        {
            { 4466.790039f, 1960.420044f, 459.144012f, 1.151920f, 192488, 192501 },     // Flag on tower
            { 4475.350098f, 1937.030029f, 459.070007f, -0.43633f, 192488, 192501 },     // Flag on tower
            { 4451.759766f, 1928.099976f, 459.075989f, -2.00713f, 192488, 192501 },     // Flag on tower
            { 4442.990234f, 1951.900024f, 459.092987f, 2.740160f, 192488, 192501 },     // Flag on tower
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
        },
        5,
        {
            { 4501.060059f, 1990.280029f, 431.157013f, 1.029740f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { 4463.830078f, 2015.180054f, 430.299988f, 1.431170f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { 4494.580078f, 1943.760010f, 435.627014f, 6.195920f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { 4437.090000f, 1985.960000f, 434.607000f, 0.870000f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { 4483.816895f, 1999.649290f, 429.788239f, 1.219750f, BATTLEFIELD_WG_NPC_GUARD_H, BATTLEFIELD_WG_NPC_GUARD_A },     // Standing Guard
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
        },
        0,
        {
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
            { 0, 0, 0, 0, 0, 0 },
        },
    },
};

struct WintergraspTowerCannonData
{
    uint32 towerEntry;
    uint8 nbTowerCannonBottom;
    Position TowerCannonBottom[5];
    uint8 nbTurretTop;
    Position TurretTop[5];
};

uint8 const WG_MAX_TOWER_CANNON = 7;

WintergraspTowerCannonData const TowerCannon[WG_MAX_TOWER_CANNON] =
{
    {
        190221,
        0,
        {
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
        },
        2,
        {
            { 5254.158203125f, 3047.79541015625f, 438.58270263671875f, 3.141592741012573242f }, // VerifiedBuild 51666
            { 5280.96923828125f, 3073.374267578125f, 438.58270263671875f, 1.65806281566619873f }, // VerifiedBuild 51666
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
        },
    },
    {
        190373,
        0,
        {
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
        },
        2,
        {
            { 5136.8427734375f, 2935.2646484375f, 439.929718017578125f, 3.106686115264892578f }, // VerifiedBuild 51666
            { 5163.5087890625f, 2960.82080078125f, 439.929718017578125f, 1.605702877044677734f }, // VerifiedBuild 51666
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
        },
    },
    {
        190377,
        0,
        {
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
        },
        2,
        {
            { 5163.86279296875f, 2721.9326171875f, 439.927734375f, 4.642575740814208984f }, // VerifiedBuild 51666
            { 5137.888671875f, 2747.526611328125f, 439.927734375f, 3.159045934677124023f }, // VerifiedBuild 51666
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
        },
    },
    {
        190378,
        0,
        {
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
        },
        2,
        {
            { 5278.27001953125f, 2605.745361328125f, 439.837890625f, 4.817108631134033203f }, // VerifiedBuild 51666
            { 5252.43603515625f, 2632.1494140625f, 439.837890625f, 3.193952560424804687f }, // VerifiedBuild 51666
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
            { 0, 0, 0, 0 },
        },
    },
    {
        190356,
        4,
        {
            { 4538.8867f, 3601.4148f, 402.886993f, 3.998462f }, // VerifiedBuild 0
            { 4579.6943f, 3605.6738f, 402.886993f, 5.651723f }, // VerifiedBuild 0
            { 4534.7104f, 3642.0645f, 402.886993f, 2.455163f }, // VerifiedBuild 0
            { 4575.2915f, 3646.363f, 402.886993f, 0.884366f }, // VerifiedBuild 0
            { 0, 0, 0, 0 },
        },
        4,
        {
            { 4583.152f, 3626.6692f, 426.539062f, 0.117806f }, // VerifiedBuild 0
            { 4559.8447f, 3598.097f, 426.539062f, 4.780807f }, // VerifiedBuild 0
            { 4531.4473f, 3621.2424f, 426.539062f, 3.268910f }, // VerifiedBuild 0
            { 4554.4956f, 3649.5588f, 426.539062f, 1.657981f }, // VerifiedBuild 0
            { 0, 0, 0, 0 },
        },
    },
    {
        190357,
        4,
        {
            { 4419.017578125f, 2802.616455078125f, 412.71405029296875f, 5.550147056579589843f }, // VerifiedBuild 51666
            { 4418.22314453125f, 2843.650146484375f, 412.71405029296875f, 0.820304751396179199f }, // VerifiedBuild 51666
            { 4377.2593f, 2842.6213f, 412.71405029296875f, 2.376623f }, // VerifiedBuild 0
            { 4377.91943359375f, 2801.678955078125f, 412.71405029296875f, 3.979350566864013671f }, // VerifiedBuild 51666
            { 0, 0, 0, 0 },
        },
        4,
        {
            { 4424.33642578125f, 2822.9638671875f, 436.36627197265625f, 6.161012172698974609f }, // VerifiedBuild 51666
            { 4397.763f, 2848.164f, 436.36627197265625f, 1.579430f }, // VerifiedBuild 0
            { 4398.64404296875f, 2796.262939453125f, 436.36627197265625f, 4.78220224380493164f }, // VerifiedBuild 51666
            { 4372.1884765625f, 2822.0888671875f, 436.36627197265625f, 3.263765573501586914f }, // VerifiedBuild 51666
            { 0, 0, 0, 0 },
        },
    },
    {
        190358,
        4,
        {
            { 4448.80712890625f, 1971.795166015625f, 442.07830810546875f, 2.0245819091796875f }, // VerifiedBuild 51666
            { 4486.32568359375f, 1954.654541015625f, 442.07830810546875f, 0.349065840244293212f }, // VerifiedBuild 51666
            { 4469.431640625f, 1917.064208984375f, 442.07830810546875f, 5.078907966613769531f }, // VerifiedBuild 51666
            { 4431.83056640625f, 1934.13720703125f, 442.07830810546875f, 3.647738218307495117f }, // VerifiedBuild 51666
            { 0, 0, 0, 0 },
        },
        4,
        {
            { 4469.7724609375f, 1967.2432861328125f, 465.73052978515625f, 1.221730470657348632f }, // VerifiedBuild 51666
            { 4482.228515625f, 1933.906494140625f, 465.73052978515625f, 5.951572895050048828f }, // VerifiedBuild 51666
            { 4436.2080078125f, 1954.7906494140625f, 465.73052978515625f, 2.740166902542114257f }, // VerifiedBuild 51666
            { 4448.705078125f, 1921.1121826171875f, 465.73052978515625f, 4.398229598999023437f }, // VerifiedBuild 51666
            { 0, 0, 0, 0 },
        },
    },
};

// *********************************************************
// *****************WorkShop Data & Element*****************
// *********************************************************

uint8 const WG_MAX_WORKSHOP = 6;

struct WGWorkshopData
{
    uint8 id;
    uint32 worldstate;
    uint8 attackText;
    uint8 takenText;
};

WGWorkshopData const WorkshopsData[WG_MAX_WORKSHOP] =
{
    // NE
    {BATTLEFIELD_WG_WORKSHOP_NE, WORLD_STATE_BATTLEFIELD_WG_WORKSHOP_NE, BATTLEFIELD_WG_TEXT_WORKSHOP_NE_ATTACK, BATTLEFIELD_WG_TEXT_WORKSHOP_NE_TAKEN},
    // NW
    {BATTLEFIELD_WG_WORKSHOP_NW, WORLD_STATE_BATTLEFIELD_WG_WORKSHOP_NW, BATTLEFIELD_WG_TEXT_WORKSHOP_NW_ATTACK, BATTLEFIELD_WG_TEXT_WORKSHOP_NW_TAKEN},
    // SE
    {BATTLEFIELD_WG_WORKSHOP_SE, WORLD_STATE_BATTLEFIELD_WG_WORKSHOP_SE, BATTLEFIELD_WG_TEXT_WORKSHOP_SE_ATTACK, BATTLEFIELD_WG_TEXT_WORKSHOP_SE_TAKEN},
    // SW
    {BATTLEFIELD_WG_WORKSHOP_SW, WORLD_STATE_BATTLEFIELD_WG_WORKSHOP_SW, BATTLEFIELD_WG_TEXT_WORKSHOP_SW_ATTACK, BATTLEFIELD_WG_TEXT_WORKSHOP_SW_TAKEN},
    // KEEP WEST - It can't be taken
    {BATTLEFIELD_WG_WORKSHOP_KEEP_WEST, WORLD_STATE_BATTLEFIELD_WG_WORKSHOP_K_W, 0, BATTLEFIELD_WG_TEXT_WORKSHOP_NE_TAKEN},
    // KEEP EAST - It can't be taken
    {BATTLEFIELD_WG_WORKSHOP_KEEP_EAST, WORLD_STATE_BATTLEFIELD_WG_WORKSHOP_K_E, 0, BATTLEFIELD_WG_TEXT_WORKSHOP_NE_TAKEN}
};

// ********************************************************************
// *         Structs using for Building,Graveyard,Workshop            *
// ********************************************************************
// Structure for different buildings that can be destroyed during battle
struct BfWGGameObjectBuilding
{
    BfWGGameObjectBuilding(BattlefieldWG* WG)
    {
        m_WG = WG;
        m_Team = TEAM_ALLIANCE;
        m_Type = 0;
        m_WorldState = 0;
        m_State = 0;
        m_damagedText = 0;
        m_destroyedText = 0;
    }

    // the team that controls this point
    TeamId m_Team;

    // WG object
    BattlefieldWG* m_WG;

    // Linked gameobject
    ObjectGuid m_Build;

    // eWGGameObjectBuildingType
    uint32 m_Type;

    // WorldState
    uint32 m_WorldState;

    // eWGGameObjectState
    uint32 m_State;

    // Name id for warning text
    uint8 m_damagedText;
    uint8 m_destroyedText;

    // GameObject associations
    GameObjectSet m_GameObjectList[2];

    // Creature associations
    GuidUnorderedSet m_CreatureBottomList[2];
    GuidUnorderedSet m_CreatureTopList[2];
    GuidUnorderedSet m_TowerCannonBottomList;
    GuidUnorderedSet m_TurretTopList;

    void Rebuild()
    {
        switch (m_Type)
        {
            case BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER:
            case BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST:
            case BATTLEFIELD_WG_OBJECTTYPE_DOOR:
            case BATTLEFIELD_WG_OBJECTTYPE_WALL:
                m_Team = m_WG->GetDefenderTeam();           // Objects that are part of the keep should be the defender's
                break;
            case BATTLEFIELD_WG_OBJECTTYPE_TOWER:
                m_Team = m_WG->GetAttackerTeam();           // The towers in the south should be the attacker's
                break;
            default:
                m_Team = TEAM_NEUTRAL;
                break;
        }

        GameObject* go = m_WG->GetGameObject(m_Build);
        if (go)
        {
            // Rebuild gameobject
            go->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING, nullptr, true);
            go->SetUInt32Value(GAMEOBJECT_FACTION, WintergraspFaction[m_Team]);
        }

        // Update worldstate
        m_State = BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT - (m_Team * 3);
        m_WG->SendUpdateWorldState(m_WorldState, m_State);
        UpdateCreatureAndGo();
    }

    // Called when associated gameobject is damaged
    void Damaged()
    {
        // Update worldstate
        m_State = BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DAMAGE - (m_Team * 3);
        m_WG->SendUpdateWorldState(m_WorldState, m_State);

        // Send warning message
        if (m_damagedText)                                       // tower damage + name
            m_WG->SendWarning(m_damagedText);

        for (GuidUnorderedSet::const_iterator itr = m_CreatureTopList[m_WG->GetAttackerTeam()].begin(); itr != m_CreatureTopList[m_WG->GetAttackerTeam()].end(); ++itr)
            if (Creature* creature = m_WG->GetCreature(*itr))
                m_WG->HideNpc(creature);

        for (GuidUnorderedSet::const_iterator itr = m_TurretTopList.begin(); itr != m_TurretTopList.end(); ++itr)
            if (Creature* creature = m_WG->GetCreature(*itr))
                m_WG->HideNpc(creature);

        if (m_Type == BATTLEFIELD_WG_OBJECTTYPE_TOWER)
            m_WG->UpdateDamagedTowerCount(m_WG->GetAttackerTeam());
    }

    // Called when associated gameobject is destroyed
    void Destroyed()
    {
        // Update worldstate
        m_State = BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DESTROY - (m_Team * 3);
        m_WG->SendUpdateWorldState(m_WorldState, m_State);

        // Warn players
        if (m_destroyedText)
            m_WG->SendWarning(m_destroyedText);

        switch (m_Type)
        {
            // Inform the global wintergrasp script of the destruction of this object
            case BATTLEFIELD_WG_OBJECTTYPE_TOWER:
                m_WG->UpdatedDestroyedTowerCount(TeamId(m_Team), m_WG->GetGameObject(m_Build));
                break;
            case BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST:
                m_WG->SetRelicInteractible(true);
                if (GameObject* go = m_WG->GetRelic())
                    go->RemoveGameObjectFlag(GO_FLAG_NOT_SELECTABLE);
                else
                    LOG_ERROR("bg.battlefield", "BattlefieldWG: Relic not found.");
                break;
            case BATTLEFIELD_WG_OBJECTTYPE_DOOR:
            case BATTLEFIELD_WG_OBJECTTYPE_WALL:
            case BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER:
                m_WG->UpdatedDestroyedTowerCount(TeamId(m_Team), m_WG->GetGameObject(m_Build));
                break;
        }

        m_WG->BrokenWallOrTower(TeamId(m_Team));
    }

    void Init(GameObject* gobj, uint32 type, uint32 worldstate, uint8 damageText, uint8 destroyText)
    {
        if (!gobj)
        {
            return;
        }

        // GameObject associated to object
        m_Build = gobj->GetGUID();

        // Type of building (WALL/TOWER/DOOR)
        m_Type = type;

        // WorldState for client (icon on map)
        m_WorldState = worldstate;

        // NameId for Warning text
        m_damagedText = damageText;
        m_destroyedText = destroyText;

        switch (m_Type)
        {
            case BATTLEFIELD_WG_OBJECTTYPE_KEEP_TOWER:
            case BATTLEFIELD_WG_OBJECTTYPE_DOOR_LAST:
            case BATTLEFIELD_WG_OBJECTTYPE_DOOR:
            case BATTLEFIELD_WG_OBJECTTYPE_WALL:
                m_Team = m_WG->GetDefenderTeam();           // Objects that are part of the keep should be the defender's
                break;
            case BATTLEFIELD_WG_OBJECTTYPE_TOWER:
                m_Team = m_WG->GetAttackerTeam();           // The towers in the south should be the attacker's
                break;
            default:
                m_Team = TEAM_NEUTRAL;
                break;
        }

        m_State = sWorldState->getWorldState(m_WorldState);
        if (gobj)
        {
            switch (m_State)
            {
                case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT:
                case BATTLEFIELD_WG_OBJECTSTATE_HORDE_INTACT:
                    gobj->SetDestructibleState(GO_DESTRUCTIBLE_REBUILDING, nullptr, true);
                    break;
                case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DESTROY:
                case BATTLEFIELD_WG_OBJECTSTATE_HORDE_DESTROY:
                    gobj->SetDestructibleState(GO_DESTRUCTIBLE_DESTROYED);
                    break;
                case BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_DAMAGE:
                case BATTLEFIELD_WG_OBJECTSTATE_HORDE_DAMAGE:
                    gobj->SetDestructibleState(GO_DESTRUCTIBLE_DAMAGED);
                    break;
            }
        }

        int32 towerid = -1;
        switch (gobj->GetEntry())
        {
            case GO_WINTERGRASP_FORTRESS_TOWER_1:
                towerid = 0;
                break;
            case GO_WINTERGRASP_FORTRESS_TOWER_2:
                towerid = 1;
                break;
            case GO_WINTERGRASP_FORTRESS_TOWER_3:
                towerid = 2;
                break;
            case GO_WINTERGRASP_FORTRESS_TOWER_4:
                towerid = 3;
                break;
            case GO_WINTERGRASP_SHADOWSIGHT_TOWER:
                towerid = 4;
                break;
            case GO_WINTERGRASP_WINTER_S_EDGE_TOWER:
                towerid = 5;
                break;
            case GO_WINTERGRASP_FLAMEWATCH_TOWER:
                towerid = 6;
                break;
        }

        if (towerid > 3) // Attacker towers
        {
            // Spawn associate gameobjects
            for (uint8 i = 0; i < AttackTowers[towerid - 4].nbObject; i++)
            {
                WintergraspObjectPositionData gobData = AttackTowers[towerid - 4].GameObject[i];
                if (GameObject* go = m_WG->SpawnGameObject(gobData.entryHorde, gobData.x, gobData.y, gobData.z, gobData.o))
                    m_GameObjectList[TEAM_HORDE].insert(go);
                if (GameObject* go = m_WG->SpawnGameObject(gobData.entryAlliance, gobData.x, gobData.y, gobData.z, gobData.o))
                    m_GameObjectList[TEAM_ALLIANCE].insert(go);
            }

            // Spawn associate npc bottom
            for (uint8 i = 0; i < AttackTowers[towerid - 4].nbCreatureBottom; i++)
            {
                WintergraspObjectPositionData creatureData = AttackTowers[towerid - 4].CreatureBottom[i];
                if (Creature* creature = m_WG->SpawnCreature(creatureData.entryHorde, creatureData.x, creatureData.y, creatureData.z, creatureData.o, TEAM_HORDE))
                    m_CreatureBottomList[TEAM_HORDE].insert(creature->GetGUID());
                if (Creature* creature = m_WG->SpawnCreature(creatureData.entryAlliance, creatureData.x, creatureData.y, creatureData.z, creatureData.o, TEAM_ALLIANCE))
                    m_CreatureBottomList[TEAM_ALLIANCE].insert(creature->GetGUID());
            }

            // Spawn associate npc top
            for (uint8 i = 0; i < AttackTowers[towerid - 4].nbCreatureTop; i++)
            {
                WintergraspObjectPositionData creatureData = AttackTowers[towerid - 4].CreatureTop[i];
                if (Creature* creature = m_WG->SpawnCreature(creatureData.entryHorde, creatureData.x, creatureData.y, creatureData.z, creatureData.o, TEAM_HORDE))
                    m_CreatureTopList[TEAM_HORDE].insert(creature->GetGUID());
                if (Creature* creature = m_WG->SpawnCreature(creatureData.entryAlliance, creatureData.x, creatureData.y, creatureData.z, creatureData.o, TEAM_ALLIANCE))
                    m_CreatureTopList[TEAM_ALLIANCE].insert(creature->GetGUID());
            }
        }

        if (towerid >= 0)
        {
            // Spawn Turret bottom
            for (uint8 i = 0; i < TowerCannon[towerid].nbTowerCannonBottom; i++)
            {
                Position turretPos = TowerCannon[towerid].TowerCannonBottom[i].GetPosition();
                if (Creature* turret = m_WG->SpawnCreature(NPC_WINTERGRASP_TOWER_CANNON, turretPos, TEAM_ALLIANCE))
                {
                    m_TowerCannonBottomList.insert(turret->GetGUID());
                    m_WG->HideNpc(turret);
                }
            }

            // Spawn Turret top
            for (uint8 i = 0; i < TowerCannon[towerid].nbTurretTop; i++)
            {
                Position towerCannonPos = TowerCannon[towerid].TurretTop[i].GetPosition();
                if (Creature* turret = m_WG->SpawnCreature(NPC_WINTERGRASP_TOWER_CANNON, towerCannonPos, TEAM_ALLIANCE))
                {
                    m_TurretTopList.insert(turret->GetGUID());
                    m_WG->HideNpc(turret);
                }
            }
            UpdateCreatureAndGo();
        }
    }

    void UpdateCreatureAndGo()
    {
        for (GuidUnorderedSet::const_iterator itr = m_CreatureTopList[m_WG->GetDefenderTeam()].begin(); itr != m_CreatureTopList[m_WG->GetDefenderTeam()].end(); ++itr)
            if (Creature* creature = m_WG->GetCreature(*itr))
                m_WG->HideNpc(creature);

        for (GuidUnorderedSet::const_iterator itr = m_CreatureTopList[m_WG->GetAttackerTeam()].begin(); itr != m_CreatureTopList[m_WG->GetAttackerTeam()].end(); ++itr)
            if (Creature* creature = m_WG->GetCreature(*itr))
                m_WG->ShowNpc(creature, true);

        for (GuidUnorderedSet::const_iterator itr = m_CreatureBottomList[m_WG->GetDefenderTeam()].begin(); itr != m_CreatureBottomList[m_WG->GetDefenderTeam()].end(); ++itr)
            if (Creature* creature = m_WG->GetCreature(*itr))
                m_WG->HideNpc(creature);

        for (GuidUnorderedSet::const_iterator itr = m_CreatureBottomList[m_WG->GetAttackerTeam()].begin(); itr != m_CreatureBottomList[m_WG->GetAttackerTeam()].end(); ++itr)
            if (Creature* creature = m_WG->GetCreature(*itr))
                m_WG->ShowNpc(creature, true);

        for (GameObjectSet::const_iterator itr = m_GameObjectList[m_WG->GetDefenderTeam()].begin(); itr != m_GameObjectList[m_WG->GetDefenderTeam()].end(); ++itr)
            (*itr)->SetRespawnTime(RESPAWN_ONE_DAY);

        for (GameObjectSet::const_iterator itr = m_GameObjectList[m_WG->GetAttackerTeam()].begin(); itr != m_GameObjectList[m_WG->GetAttackerTeam()].end(); ++itr)
            (*itr)->SetRespawnTime(RESPAWN_IMMEDIATELY);
    }

    void UpdateTurretAttack(bool disable)
    {
        GameObject* build = m_WG->GetGameObject(m_Build);
        if (!build)
            return;

        uint32 faction = 0;
        switch (build->GetEntry())
        {
            case GO_WINTERGRASP_FORTRESS_TOWER_1:
            case GO_WINTERGRASP_FORTRESS_TOWER_2:
            case GO_WINTERGRASP_FORTRESS_TOWER_3:
            case GO_WINTERGRASP_FORTRESS_TOWER_4:
                faction = WintergraspFaction[m_WG->GetDefenderTeam()];
                break;
            case GO_WINTERGRASP_SHADOWSIGHT_TOWER:
            case GO_WINTERGRASP_WINTER_S_EDGE_TOWER:
            case GO_WINTERGRASP_FLAMEWATCH_TOWER:
                faction = WintergraspFaction[m_WG->GetAttackerTeam()];
                break;
        }

        for (GuidUnorderedSet::const_iterator itr = m_TowerCannonBottomList.begin(); itr != m_TowerCannonBottomList.end(); ++itr)
        {
            if (Creature* creature = m_WG->GetCreature(*itr))
            {
                creature->SetFaction(faction);
                if (disable)
                    m_WG->HideNpc(creature);
                else
                    m_WG->ShowNpc(creature, true);
            }
        }

        for (GuidUnorderedSet::const_iterator itr = m_TurretTopList.begin(); itr != m_TurretTopList.end(); ++itr)
        {
            if (Creature* creature = m_WG->GetCreature(*itr))
            {
                creature->SetFaction(faction);
                if (disable)
                    m_WG->HideNpc(creature);
                else
                    m_WG->ShowNpc(creature, true);
            }
        }
    }

    void Save()
    {
        sWorldState->setWorldState(m_WorldState, m_State);
    }
};

struct WGWorkshop
{
    // pointer to the battlefield that the workshop belongs to
    BattlefieldWG* bf;
    // id of the workshop, useful to retrieve data of the WorkshopsData array
    uint8 workshopId;
    // team that controls the node
    TeamId teamControl;
    // for worldstate
    uint32 state;

    WGWorkshop(BattlefieldWG* _bf, uint8 _workshopId)
    {
        ASSERT(_bf || _workshopId < WG_MAX_WORKSHOP);

        bf = _bf;
        workshopId = _workshopId;
        teamControl = TEAM_NEUTRAL;
        state = BATTLEFIELD_WG_OBJECTSTATE_NONE;
    }

    void GiveControlTo(TeamId team, bool init /* for first call in setup*/)
    {
        switch (team)
        {
            case TEAM_NEUTRAL:
                {
                    // Send warning message to all player to inform a faction attack to a workshop
                    // alliance / horde attacking a workshop
                    bf->SendWarning(teamControl ? WorkshopsData[workshopId].attackText : (WorkshopsData[workshopId].attackText + 2));

                    // Updating worldstate, update icon to neutral
                    state = BATTLEFIELD_WG_OBJECTSTATE_NEUTRAL_INTACT;
                    bf->SendUpdateWorldState(WorkshopsData[workshopId].worldstate, state);

                    // Found associate graveyard and update it
                    if (workshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
                        if (bf->GetGraveyardById(workshopId))
                            bf->GetGraveyardById(workshopId)->GiveControlTo(team);

                    teamControl = team;
                    break;
                }
            case TEAM_ALLIANCE:
            case TEAM_HORDE:
                {
                    // Updating worldstate
                    state = team == TEAM_ALLIANCE ? BATTLEFIELD_WG_OBJECTSTATE_ALLIANCE_INTACT : BATTLEFIELD_WG_OBJECTSTATE_HORDE_INTACT;
                    bf->SendUpdateWorldState(WorkshopsData[workshopId].worldstate, state);

                    // Warning message
                    if (!init)                              // workshop taken - alliance
                        bf->SendWarning(team == TEAM_ALLIANCE ? WorkshopsData[workshopId].takenText : (WorkshopsData[workshopId].takenText + 2));

                    // Found associate graveyard and update it
                    if (workshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
                        if (bf->GetGraveyardById(workshopId))
                            bf->GetGraveyardById(workshopId)->GiveControlTo(team);

                    teamControl = team;
                    break;
                }
        }

        if (!init)
        {
            bf->UpdateCounterVehicle(false);
            bf->CapturePointTaken(bf->GetAreaByGraveyardId(workshopId));
        }
    }

    void UpdateGraveyardAndWorkshop()
    {
        if (workshopId < BATTLEFIELD_WG_WORKSHOP_KEEP_WEST)
            bf->GetGraveyardById(workshopId)->GiveControlTo(TeamId(teamControl));
        else
            GiveControlTo(bf->GetDefenderTeam(), true);
    }

    void Save()
    {
        sWorldState->setWorldState(WorkshopsData[workshopId].worldstate, state);
    }
};

#endif
