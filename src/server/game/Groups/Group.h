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

#ifndef AZEROTHCORE_GROUP_H
#define AZEROTHCORE_GROUP_H

#include "DBCEnums.h"
#include "DataMap.h"
#include "GroupRefMgr.h"
#include "LootMgr.h"
#include "QueryResult.h"
#include "SharedDefines.h"
#include <functional>

class Battlefield;
class Battleground;
class Creature;
class GroupReference;
class InstanceSave;
class Map;
class Player;
class Unit;
class WorldObject;
class WorldPacket;
class WorldSession;

struct MapEntry;

#define MAXGROUPSIZE 5
#define MAXRAIDSIZE 40
#define MAX_RAID_SUBGROUPS MAXRAIDSIZE/MAXGROUPSIZE
#define TARGETICONCOUNT 8

enum RollVote : uint8
{
    PASS              = 0,
    NEED              = 1,
    GREED             = 2,
    DISENCHANT        = 3,
    NOT_EMITED_YET    = 4,
    NOT_VALID         = 5
};

enum GroupMemberOnlineStatus
{
    MEMBER_STATUS_OFFLINE   = 0x0000,
    MEMBER_STATUS_ONLINE    = 0x0001,                       // Lua_UnitIsConnected
    MEMBER_STATUS_PVP       = 0x0002,                       // Lua_UnitIsPVP
    MEMBER_STATUS_DEAD      = 0x0004,                       // Lua_UnitIsDead
    MEMBER_STATUS_GHOST     = 0x0008,                       // Lua_UnitIsGhost
    MEMBER_STATUS_PVP_FFA   = 0x0010,                       // Lua_UnitIsPVPFreeForAll
    MEMBER_STATUS_UNK3      = 0x0020,                       // used in calls from Lua_GetPlayerMapPosition/Lua_GetBattlefieldFlagPosition
    MEMBER_STATUS_AFK       = 0x0040,                       // Lua_UnitIsAFK
    MEMBER_STATUS_DND       = 0x0080,                       // Lua_UnitIsDND
};

enum GroupMemberFlags
{
    MEMBER_FLAG_ASSISTANT   = 0x01,
    MEMBER_FLAG_MAINTANK    = 0x02,
    MEMBER_FLAG_MAINASSIST  = 0x04,
};

enum GroupMemberAssignment
{
    GROUP_ASSIGN_MAINTANK   = 0,
    GROUP_ASSIGN_MAINASSIST = 1,
};

enum GroupType
{
    GROUPTYPE_NORMAL         = 0x00,
    GROUPTYPE_BG             = 0x01,
    GROUPTYPE_RAID           = 0x02,
    GROUPTYPE_BGRAID         = GROUPTYPE_BG | GROUPTYPE_RAID, // mask
    GROUPTYPE_LFG_RESTRICTED = 0x04, // Script_HasLFGRestrictions()
    GROUPTYPE_LFG            = 0x08,
    // 0x10, leave/change group?, I saw this flag when leaving group and after leaving BG while in group
    // GROUPTYPE_ONE_PERSON_PARTY   = 0x20, 4.x Script_IsOnePersonParty()
    // GROUPTYPE_EVERYONE_ASSISTANT = 0x40  4.x Script_IsEveryoneAssistant()
};

enum GroupUpdateFlags
{
    GROUP_UPDATE_FLAG_NONE              = 0x00000000,       // nothing
    GROUP_UPDATE_FLAG_STATUS            = 0x00000001,       // uint16, flags
    GROUP_UPDATE_FLAG_CUR_HP            = 0x00000002,       // uint32
    GROUP_UPDATE_FLAG_MAX_HP            = 0x00000004,       // uint32
    GROUP_UPDATE_FLAG_POWER_TYPE        = 0x00000008,       // uint8
    GROUP_UPDATE_FLAG_CUR_POWER         = 0x00000010,       // uint16
    GROUP_UPDATE_FLAG_MAX_POWER         = 0x00000020,       // uint16
    GROUP_UPDATE_FLAG_LEVEL             = 0x00000040,       // uint16
    GROUP_UPDATE_FLAG_ZONE              = 0x00000080,       // uint16
    GROUP_UPDATE_FLAG_POSITION          = 0x00000100,       // uint16, uint16
    GROUP_UPDATE_FLAG_AURAS             = 0x00000200,       // uint64 mask, for each bit set uint32 spellid + uint8 unk
    GROUP_UPDATE_FLAG_PET_GUID          = 0x00000400,       // uint64 pet guid
    GROUP_UPDATE_FLAG_PET_NAME          = 0x00000800,       // pet name, nullptr terminated string
    GROUP_UPDATE_FLAG_PET_MODEL_ID      = 0x00001000,       // uint16, model id
    GROUP_UPDATE_FLAG_PET_CUR_HP        = 0x00002000,       // uint32 pet cur health
    GROUP_UPDATE_FLAG_PET_MAX_HP        = 0x00004000,       // uint32 pet max health
    GROUP_UPDATE_FLAG_PET_POWER_TYPE    = 0x00008000,       // uint8 pet power type
    GROUP_UPDATE_FLAG_PET_CUR_POWER     = 0x00010000,       // uint16 pet cur power
    GROUP_UPDATE_FLAG_PET_MAX_POWER     = 0x00020000,       // uint16 pet max power
    GROUP_UPDATE_FLAG_PET_AURAS         = 0x00040000,       // uint64 mask, for each bit set uint32 spellid + uint8 unk, pet auras...
    GROUP_UPDATE_FLAG_VEHICLE_SEAT      = 0x00080000,       // uint32 vehicle_seat_id (index from VehicleSeat.dbc)
    GROUP_UPDATE_PET                    = 0x0007FC00,       // all pet flags
    GROUP_UPDATE_FULL                   = 0x0007FFFF,       // all known flags
};

enum lfgGroupFlags
{
    GROUP_LFG_FLAG_APPLY_RANDOM_BUFF        = 0x001,
    GROUP_LFG_FLAG_IS_RANDOM_INSTANCE       = 0x002,
    GROUP_LFG_FLAG_IS_HEROIC                = 0x004
};

enum DifficultyPreventionChangeType
{
    DIFFICULTY_PREVENTION_CHANGE_NONE                   = 0,
    DIFFICULTY_PREVENTION_CHANGE_RECENTLY_CHANGED       = 1,
    DIFFICULTY_PREVENTION_CHANGE_BOSS_KILLED            = 2
};

#define GROUP_UPDATE_FLAGS_COUNT          20
// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19
static const uint8 GroupUpdateLength[GROUP_UPDATE_FLAGS_COUNT] = { 0, 2, 2, 2, 1, 2, 2, 2, 2, 4, 8, 8, 1, 2, 2, 2, 1, 2, 2, 8};

class Roll : public LootValidatorRef
{
public:
    Roll(ObjectGuid _guid, LootItem const& li);
    ~Roll();
    void setLoot(Loot* pLoot);
    Loot* getLoot();
    void targetObjectBuildLink();

    ObjectGuid itemGUID;
    uint32 itemid;
    int32  itemRandomPropId;
    uint32 itemRandomSuffix;
    uint8 itemCount;
    using PlayerVote = std::map<ObjectGuid, RollVote>;
    PlayerVote playerVote;                              //vote position correspond with player position (in group)
    uint8 totalPlayersRolling;
    uint8 totalNeed;
    uint8 totalGreed;
    uint8 totalPass;
    uint8 itemSlot;
    uint8 rollVoteMask;
};

/** request member stats checken **/
/** todo: uninvite people that not accepted invite **/
class Group
{
public:
    struct MemberSlot
    {
        ObjectGuid  guid;
        std::string name;
        uint8       group;
        uint8       flags;
        uint8       roles;
    };
    using MemberSlotList = std::list<MemberSlot>;
    using member_citerator = MemberSlotList::const_iterator;
protected:
    using member_witerator = MemberSlotList::iterator;
    using InvitesList = std::set<Player*>;
    using Rolls = std::vector<Roll*>;
public:
    Group();
    ~Group();

    // group manipulation methods
    bool   Create(Player* leader);
    bool   LoadGroupFromDB(Field* field);
    void   LoadMemberFromDB(ObjectGuid::LowType guidLow, uint8 memberFlags, uint8 subgroup, uint8 roles);
    bool   AddInvite(Player* player);
    void   RemoveInvite(Player* player);
    void   RemoveAllInvites();
    bool   AddLeaderInvite(Player* player);
    bool   AddMember(Player* player);
    bool   RemoveMember(ObjectGuid guid, const RemoveMethod& method = GROUP_REMOVEMETHOD_DEFAULT, ObjectGuid kicker = ObjectGuid::Empty, const char* reason = nullptr);
    void   ChangeLeader(ObjectGuid guid);
    void   SetLootMethod(LootMethod method);
    void   SetLooterGuid(ObjectGuid guid);
    void   SetMasterLooterGuid(ObjectGuid guid);
    void   UpdateLooterGuid(WorldObject* pLootedObject, bool ifneed = false);
    void   SetLootThreshold(ItemQualities threshold);
    void   Disband(bool hideDestroy = false);
    void   SetLfgRoles(ObjectGuid guid, const uint8 roles);

    // properties accessories
    bool IsFull() const;
    bool isLFGGroup(bool restricted = false)  const;
    bool isRaidGroup() const;
    bool isBFGroup()   const;
    bool isBGGroup()   const;
    bool IsCreated()   const;
    GroupType GetGroupType() const;
    ObjectGuid GetLeaderGUID() const;
    Player* GetLeader();
    ObjectGuid GetGUID() const;
    const char* GetLeaderName() const;
    LootMethod GetLootMethod() const;
    ObjectGuid GetLooterGuid() const;
    ObjectGuid GetMasterLooterGuid() const;
    ItemQualities GetLootThreshold() const;

    // member manipulation methods
    bool IsMember(ObjectGuid guid) const;
    bool IsLeader(ObjectGuid guid) const;
    ObjectGuid GetMemberGUID(const std::string& name);
    bool IsAssistant(ObjectGuid guid) const;

    Player* GetInvited(ObjectGuid guid) const;
    Player* GetInvited(const std::string& name) const;

    bool SameSubGroup(ObjectGuid guid1, ObjectGuid guid2) const;
    bool SameSubGroup(ObjectGuid guid1, MemberSlot const* slot2) const;
    bool SameSubGroup(Player const* member1, Player const* member2) const;
    bool HasFreeSlotSubGroup(uint8 subgroup) const;

    MemberSlotList const& GetMemberSlots() const { return m_memberSlots; }
    GroupReference* GetFirstMember() { return m_memberMgr.getFirst(); }
    GroupReference const* GetFirstMember() const { return m_memberMgr.getFirst(); }
    uint32 GetMembersCount() const { return m_memberSlots.size(); }
    uint32 GetInviteeCount() const { return m_invitees.size(); }

    uint8 GetMemberGroup(ObjectGuid guid) const;

    void ConvertToLFG(bool restricted = true);
    bool CheckLevelForRaid();
    void ConvertToRaid();

    void SetBattlegroundGroup(Battleground* bg);
    void SetBattlefieldGroup(Battlefield* bf);
    GroupJoinBattlegroundResult CanJoinBattlegroundQueue(Battleground const* bgTemplate, BattlegroundQueueTypeId bgQueueTypeId, uint32 MinPlayerCount, uint32 MaxPlayerCount, bool isRated, uint32 arenaSlot);

    void ChangeMembersGroup(ObjectGuid guid, uint8 group);
    void SetTargetIcon(uint8 id, ObjectGuid whoGuid, ObjectGuid targetGuid);
    void SetGroupMemberFlag(ObjectGuid guid, bool apply, GroupMemberFlags flag);
    void RemoveUniqueGroupMemberFlag(GroupMemberFlags flag);

    Difficulty GetDifficulty(bool isRaid) const;
    Difficulty GetDungeonDifficulty() const;
    Difficulty GetRaidDifficulty() const;
    void SetDungeonDifficulty(Difficulty difficulty);
    void SetRaidDifficulty(Difficulty difficulty);
    uint16 InInstance();
    void ResetInstances(uint8 method, bool isRaid, Player* leader);

    // -no description-
    //void SendInit(WorldSession* session);
    void SendTargetIconList(WorldSession* session);
    void SendUpdate();
    void SendUpdateToPlayer(ObjectGuid playerGUID, MemberSlot* slot = nullptr);
    void UpdatePlayerOutOfRange(Player* player);
    // ignore: GUID of player that will be ignored
    void BroadcastPacket(WorldPacket const* packet, bool ignorePlayersInBGRaid, int group = -1, ObjectGuid ignore = ObjectGuid::Empty);
    void BroadcastReadyCheck(WorldPacket const* packet);
    void OfflineReadyCheck();

    /*********************************************************/
    /***                   LOOT SYSTEM                     ***/
    /*********************************************************/

    bool isRollLootActive() const;
    void SendLootStartRoll(uint32 CountDown, uint32 mapid, const Roll& r);
    void SendLootStartRollToPlayer(uint32 countDown, uint32 mapId, Player* p, bool canNeed, Roll const& r);
    void SendLootRoll(ObjectGuid SourceGuid, ObjectGuid TargetGuid, uint8 RollNumber, uint8 RollType, const Roll& r, bool autoPass = false);
    void SendLootRollWon(ObjectGuid SourceGuid, ObjectGuid TargetGuid, uint8 RollNumber, uint8 RollType, const Roll& r);
    void SendLootAllPassed(Roll const& roll);
    void SendLooter(Creature* creature, Player* pLooter);
    void GroupLoot(Loot* loot, WorldObject* pLootedObject);
    void NeedBeforeGreed(Loot* loot, WorldObject* pLootedObject);
    void MasterLoot(Loot* loot, WorldObject* pLootedObject);
    Rolls::iterator GetRoll(ObjectGuid Guid);
    void CountTheRoll(Rolls::iterator roll, Map* allowedMap);
    bool CountRollVote(ObjectGuid playerGUID, ObjectGuid Guid, uint8 Choise);
    void EndRoll(Loot* loot, Map* allowedMap);

    // related to disenchant rolls
    void ResetMaxEnchantingLevel();

    void LinkMember(GroupReference* pRef);

    // FG: evil hacks
    void BroadcastGroupUpdate(void);

    // LFG
    void AddLfgBuffFlag() { m_lfgGroupFlags |= GROUP_LFG_FLAG_APPLY_RANDOM_BUFF; }
    void AddLfgRandomInstanceFlag() { m_lfgGroupFlags |= GROUP_LFG_FLAG_IS_RANDOM_INSTANCE; }
    void AddLfgHeroicFlag() { m_lfgGroupFlags |= GROUP_LFG_FLAG_IS_HEROIC; }
    bool IsLfgWithBuff() const { return isLFGGroup() && (m_lfgGroupFlags & GROUP_LFG_FLAG_APPLY_RANDOM_BUFF); }
    bool IsLfgRandomInstance() const { return isLFGGroup() && (m_lfgGroupFlags & GROUP_LFG_FLAG_IS_RANDOM_INSTANCE); }
    bool IsLfgHeroic() const { return isLFGGroup() && (m_lfgGroupFlags & GROUP_LFG_FLAG_IS_HEROIC); }

    // Difficulty Change
    uint32 GetDifficultyChangePreventionTime() const;
    DifficultyPreventionChangeType GetDifficultyChangePreventionReason() const { return _difficultyChangePreventionType; }
    void SetDifficultyChangePrevention(DifficultyPreventionChangeType type);
    void DoForAllMembers(std::function<void(Player*)> const& worker);

    DataMap CustomData;

protected:
    void _homebindIfInstance(Player* player);
    void _cancelHomebindIfInstance(Player* player);

    void _initRaidSubGroupsCounter();
    member_citerator _getMemberCSlot(ObjectGuid Guid) const;
    member_witerator _getMemberWSlot(ObjectGuid Guid);
    void SubGroupCounterIncrease(uint8 subgroup);
    void SubGroupCounterDecrease(uint8 subgroup);
    void ToggleGroupMemberFlag(member_witerator slot, uint8 flag, bool apply);

    MemberSlotList      m_memberSlots;
    GroupRefMgr     m_memberMgr;
    InvitesList         m_invitees;
    ObjectGuid          m_leaderGuid;
    std::string         m_leaderName;
    GroupType           m_groupType;
    Difficulty          m_dungeonDifficulty;
    Difficulty          m_raidDifficulty;
    Battlefield*        m_bfGroup;
    Battleground*       m_bgGroup;
    ObjectGuid          m_targetIcons[TARGETICONCOUNT];
    LootMethod          m_lootMethod;
    ItemQualities       m_lootThreshold;
    ObjectGuid          m_looterGuid;
    ObjectGuid          m_masterLooterGuid;
    Rolls               RollId;
    uint8*              m_subGroupsCounts;
    ObjectGuid          m_guid;
    uint32              m_counter;                      // used only in SMSG_GROUP_LIST
    uint32              m_maxEnchantingLevel;
    uint8               m_lfgGroupFlags;

    // Xinef: change difficulty prevention
    uint32 _difficultyChangePreventionTime;
    DifficultyPreventionChangeType _difficultyChangePreventionType;
};
#endif
