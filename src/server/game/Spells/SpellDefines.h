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

#ifndef SPELLDEFINES_H
#define SPELLDEFINES_H

#include "Define.h"
#include <vector>

enum SpellInterruptFlags
{
    SPELL_INTERRUPT_FLAG_MOVEMENT     = 0x01, // why need this for instant?
    SPELL_INTERRUPT_FLAG_PUSH_BACK    = 0x02, // push back
    SPELL_INTERRUPT_FLAG_UNK3         = 0x04, // any info?
    SPELL_INTERRUPT_FLAG_INTERRUPT    = 0x08, // interrupt
    SPELL_INTERRUPT_FLAG_ABORT_ON_DMG = 0x10,               // _complete_ interrupt on direct damage
    SPELL_INTERRUPT_UNK               = 0x20                // unk, 564 of 727 spells having this spell start with "Glyph"
};

// See SpellAuraInterruptFlags for other values definitions
enum SpellChannelInterruptFlags
{
    CHANNEL_INTERRUPT_FLAG_INTERRUPT    = 0x08,  // interrupt
    CHANNEL_FLAG_DELAY                  = 0x4000
};

enum SpellAuraInterruptFlags
{
    AURA_INTERRUPT_FLAG_HITBYSPELL          = 0x00000001,   // 0    removed when getting hit by a negative spell?
    AURA_INTERRUPT_FLAG_TAKE_DAMAGE         = 0x00000002,   // 1    removed by any damage
    AURA_INTERRUPT_FLAG_CAST                = 0x00000004,   // 2    cast any spells
    AURA_INTERRUPT_FLAG_MOVE                = 0x00000008,   // 3    removed by any movement
    AURA_INTERRUPT_FLAG_TURNING             = 0x00000010,   // 4    removed by any turning
    AURA_INTERRUPT_FLAG_JUMP                = 0x00000020,   // 5    removed by entering combat
    AURA_INTERRUPT_FLAG_NOT_MOUNTED         = 0x00000040,   // 6    removed by dismounting
    AURA_INTERRUPT_FLAG_NOT_ABOVEWATER      = 0x00000080,   // 7    removed by entering water
    AURA_INTERRUPT_FLAG_NOT_UNDERWATER      = 0x00000100,   // 8    removed by leaving water
    AURA_INTERRUPT_FLAG_NOT_SHEATHED        = 0x00000200,   // 9    removed by unsheathing
    AURA_INTERRUPT_FLAG_TALK                = 0x00000400,   // 10   talk to npc / loot? action on creature
    AURA_INTERRUPT_FLAG_USE                 = 0x00000800,   // 11   mine/use/open action on gameobject
    AURA_INTERRUPT_FLAG_MELEE_ATTACK        = 0x00001000,   // 12   removed by attacking
    AURA_INTERRUPT_FLAG_SPELL_ATTACK        = 0x00002000,   // 13   ???
    AURA_INTERRUPT_FLAG_UNK14               = 0x00004000,   // 14
    AURA_INTERRUPT_FLAG_TRANSFORM           = 0x00008000,   // 15   removed by transform?
    AURA_INTERRUPT_FLAG_UNK16               = 0x00010000,   // 16
    AURA_INTERRUPT_FLAG_MOUNT               = 0x00020000,   // 17   misdirect, aspect, swim speed
    AURA_INTERRUPT_FLAG_NOT_SEATED          = 0x00040000,   // 18   removed by standing up (used by food and drink mostly and sleep/Fake Death like)
    AURA_INTERRUPT_FLAG_CHANGE_MAP          = 0x00080000,   // 19   leaving map/getting teleported
    AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION    = 0x00100000,   // 20   removed by auras that make you invulnerable, or make other to lose selection on you
    AURA_INTERRUPT_FLAG_UNK21               = 0x00200000,   // 21
    AURA_INTERRUPT_FLAG_TELEPORTED          = 0x00400000,   // 22
    AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT    = 0x00800000,   // 23   removed by entering pvp combat
    AURA_INTERRUPT_FLAG_DIRECT_DAMAGE       = 0x01000000,   // 24   removed by any direct damage
    AURA_INTERRUPT_FLAG_LANDING             = 0x02000000,   // 25   removed by hitting the ground
    AURA_INTERRUPT_FLAG_LEAVE_COMBAT        = 0x80000000,   // 31   removed by leaving combat

    AURA_INTERRUPT_FLAG_NOT_VICTIM = (AURA_INTERRUPT_FLAG_HITBYSPELL | AURA_INTERRUPT_FLAG_TAKE_DAMAGE | AURA_INTERRUPT_FLAG_DIRECT_DAMAGE),
};

enum SpellModOp
{
    SPELLMOD_DAMAGE                 = 0,
    SPELLMOD_DURATION               = 1,
    SPELLMOD_THREAT                 = 2,
    SPELLMOD_EFFECT1                = 3,
    SPELLMOD_CHARGES                = 4,
    SPELLMOD_RANGE                  = 5,
    SPELLMOD_RADIUS                 = 6,
    SPELLMOD_CRITICAL_CHANCE        = 7,
    SPELLMOD_ALL_EFFECTS            = 8,
    SPELLMOD_NOT_LOSE_CASTING_TIME  = 9,
    SPELLMOD_CASTING_TIME           = 10,
    SPELLMOD_COOLDOWN               = 11,
    SPELLMOD_EFFECT2                = 12,
    SPELLMOD_IGNORE_ARMOR           = 13,
    SPELLMOD_COST                   = 14,
    SPELLMOD_CRIT_DAMAGE_BONUS      = 15,
    SPELLMOD_RESIST_MISS_CHANCE     = 16,
    SPELLMOD_JUMP_TARGETS           = 17,
    SPELLMOD_CHANCE_OF_SUCCESS      = 18,
    SPELLMOD_ACTIVATION_TIME        = 19,
    SPELLMOD_DAMAGE_MULTIPLIER      = 20,
    SPELLMOD_GLOBAL_COOLDOWN        = 21,
    SPELLMOD_DOT                    = 22,
    SPELLMOD_EFFECT3                = 23,
    SPELLMOD_BONUS_MULTIPLIER       = 24,
    // spellmod 25
    SPELLMOD_PROC_PER_MINUTE        = 26,
    SPELLMOD_VALUE_MULTIPLIER       = 27,
    SPELLMOD_RESIST_DISPEL_CHANCE   = 28,
    SPELLMOD_CRIT_DAMAGE_BONUS_2    = 29, //one not used spell
    SPELLMOD_SPELL_COST_REFUND_ON_FAIL = 30
};

#define MAX_SPELLMOD 32

enum SpellValueMod
{
    SPELLVALUE_BASE_POINT0,
    SPELLVALUE_BASE_POINT1,
    SPELLVALUE_BASE_POINT2,
    SPELLVALUE_RADIUS_MOD,
    SPELLVALUE_MAX_TARGETS,
    SPELLVALUE_AURA_STACK,
    SPELLVALUE_AURA_DURATION,
    SPELLVALUE_FORCED_CRIT_RESULT,
    SPELLVALUE_MISCVALUE0,
    SPELLVALUE_MISCVALUE1,
    SPELLVALUE_MISCVALUE2,
};

enum SpellFacingFlags
{
    SPELL_FACING_FLAG_INFRONT = 0x0001
};

enum TriggerCastFlags
{
    TRIGGERED_NONE                                  = 0x00000000,   //! Not triggered
    TRIGGERED_IGNORE_GCD                            = 0x00000001,   //! Will ignore GCD
    TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD          = 0x00000002,   //! Will ignore Spell and Category cooldowns
    TRIGGERED_IGNORE_POWER_AND_REAGENT_COST         = 0x00000004,   //! Will ignore power and reagent cost
    TRIGGERED_IGNORE_CAST_ITEM                      = 0x00000008,   //! Will not take away cast item or update related achievement criteria
    TRIGGERED_IGNORE_AURA_SCALING                   = 0x00000010,   //! Will ignore aura scaling
    TRIGGERED_IGNORE_CAST_IN_PROGRESS               = 0x00000020,   //! Will not check if a current cast is in progress
    TRIGGERED_IGNORE_COMBO_POINTS                   = 0x00000040,   //! Will ignore combo point requirement
    TRIGGERED_CAST_DIRECTLY                         = 0x00000080,   //! In Spell::prepare, will be cast directly without setting containers for executed spell
    TRIGGERED_IGNORE_AURA_INTERRUPT_FLAGS           = 0x00000100,   //! Will ignore interruptible aura's at cast
    TRIGGERED_IGNORE_SET_FACING                     = 0x00000200,   //! Will not adjust facing to target (if any)
    TRIGGERED_IGNORE_SHAPESHIFT                     = 0x00000400,   //! Will ignore shapeshift checks
    TRIGGERED_IGNORE_CASTER_AURASTATE               = 0x00000800,   //! Will ignore caster aura states including combat requirements and death state
    TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE   = 0x00002000,   //! Will ignore mounted/on vehicle restrictions
    TRIGGERED_IGNORE_CASTER_AURAS                   = 0x00010000,   //! Will ignore caster aura restrictions or requirements
    TRIGGERED_DISALLOW_PROC_EVENTS                  = 0x00020000,   //! Disallows proc events from triggered spell (default)
    TRIGGERED_DONT_REPORT_CAST_ERROR                = 0x00040000,   //! Will return SPELL_FAILED_DONT_REPORT in CheckCast functions
    TRIGGERED_FULL_MASK                             = 0x0007FFFF,   //! Used when doing CastSpell with triggered == true
    TRIGGERED_IGNORE_EQUIPPED_ITEM_REQUIREMENT      = 0x00080000,   //! Will ignore equipped item requirements
    TRIGGERED_NO_PERIODIC_RESET                     = 0x00100000,   //! Periodic aura tick wont be reset on override
    TRIGGERED_IGNORE_EFFECTS                        = 0x00200000,   //! Ignore spell effects - used for ritual portals
    TRIGGERED_FULL_DEBUG_MASK                       = 0xFFFFFFFF
};

enum SpellImmuneBlockType
{
    SPELL_BLOCK_TYPE_ALL        = 0,
    SPELL_BLOCK_TYPE_POSITIVE   = 1,
};

using CustomSpellValueMod = std::pair<SpellValueMod, int32>;
class CustomSpellValues : public std::vector<CustomSpellValueMod>
{
public:
    void AddSpellMod(SpellValueMod mod, int32 value)
    {
        push_back(std::make_pair(mod, value));
    }
};

struct SpellImmune
{
    SpellImmune() : spellId(0), type(IMMUNITY_EFFECT), blockType(SPELL_BLOCK_TYPE_ALL) { }

    uint32 spellId;
    uint32 type;
    uint32 blockType;
};

using SpellImmuneList = std::vector<SpellImmune>;
#endif // SPELLDEFINES_H
