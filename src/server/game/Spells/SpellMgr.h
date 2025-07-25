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

#ifndef _SPELLMGR_H
#define _SPELLMGR_H

// For static or at-server-startup loaded spell data

#include "Common.h"
#include "Log.h"
#include "SharedDefines.h"
#include "Unit.h"

class SpellInfo;
class Player;
class Unit;
class ProcEventInfo;
struct SkillLineAbilityEntry;

#define SPELL_RELIC_COOLDOWN      1

// only used in code
enum SpellCategories
{
    SPELLCATEGORY_HEALTH_MANA_POTIONS = 4,
    SPELLCATEGORY_DEVOUR_MAGIC        = 12,
    SPELLCATEGORY_JUDGEMENT           = 1210,               // Judgement (seal trigger)
    SPELLCATEGORY_FOOD             = 11,
    SPELLCATEGORY_DRINK            = 59,
};

//SpellFamilyFlags
enum SpellFamilyFlag
{
    // SPELLFAMILYFLAG  = SpellFamilyFlags[0]
    // SPELLFAMILYFLAG1 = SpellFamilyFlags[1]
    // SPELLFAMILYFLAG2 = SpellFamilyFlags[2]

    // Rogue
    SPELLFAMILYFLAG_ROGUE_VANISH            = 0x00000800,
    SPELLFAMILYFLAG_ROGUE_VAN_EVAS_SPRINT   = 0x00000860,    // Vanish, Evasion, Sprint
    SPELLFAMILYFLAG1_ROGUE_COLDB_SHADOWSTEP = 0x00000240,    // Cold Blood, Shadowstep
    SPELLFAMILYFLAG_ROGUE_KICK              = 0x00000010,   // Kick
    SPELLFAMILYFLAG1_ROGUE_DISMANTLE        = 0x00100000,   // Dismantle
    SPELLFAMILYFLAG_ROGUE_BLADE_FLURRY      = 0x40000000,   // Blade Flurry
    SPELLFAMILYFLAG1_ROGUE_BLADE_FLURRY     = 0x00000800,   // Blade Flurry

    // Warrior
    SPELLFAMILYFLAG_WARRIOR_CHARGE          = 0x00000001,
    SPELLFAMILYFLAG_WARRIOR_SLAM            = 0x00200000,
    SPELLFAMILYFLAG_WARRIOR_EXECUTE         = 0x20000000,
    SPELLFAMILYFLAG_WARRIOR_CONCUSSION_BLOW = 0x04000000,

    // Warlock
    SPELLFAMILYFLAG_WARLOCK_LIFETAP         = 0x00040000,

    // Druid
    SPELLFAMILYFLAG2_DRUID_STARFALL         = 0x00000100,

    // Paladin
    SPELLFAMILYFLAG1_PALADIN_DIVINESTORM    = 0x00020000,

    // Shaman
    SPELLFAMILYFLAG_SHAMAN_FROST_SHOCK      = 0x80000000,
    SPELLFAMILYFLAG_SHAMAN_HEALING_STREAM   = 0x00002000,
    SPELLFAMILYFLAG_SHAMAN_MANA_SPRING      = 0x00004000,
    SPELLFAMILYFLAG2_SHAMAN_LAVA_LASH       = 0x00000004,
    SPELLFAMILYFLAG_SHAMAN_FIRE_NOVA        = 0x28000000,

    // Deathknight
    SPELLFAMILYFLAG_DK_DEATH_STRIKE         = 0x00000010,
    SPELLFAMILYFLAG_DK_DEATH_COIL           = 0x00002000,

    /// @todo: Figure out a more accurate name for the following familyflag(s)
    SPELLFAMILYFLAG_SHAMAN_TOTEM_EFFECTS    = 0x04000000,  // Seems to be linked to most totems and some totem effects
};

#define SPELL_LINKED_MAX_SPELLS  200000

enum SpellLinkedType
{
    SPELL_LINK_CAST     = 0,            // +: cast; -: remove
    SPELL_LINK_HIT      = 1 * 200000,
    SPELL_LINK_AURA     = 2 * 200000,   // +: aura; -: immune
    SPELL_LINK_REMOVE   = 0,
};

// Spell proc event related declarations (accessed using SpellMgr functions)
enum ProcFlags
{
    PROC_FLAG_NONE                            = 0x00000000,

    PROC_FLAG_KILLED                          = 0x00000001,    // 00 Killed by agressor - not sure about this flag
    PROC_FLAG_KILL                            = 0x00000002,    // 01 Kill target (in most cases need XP/Honor reward)

    PROC_FLAG_DONE_MELEE_AUTO_ATTACK          = 0x00000004,    // 02 Done melee auto attack
    PROC_FLAG_TAKEN_MELEE_AUTO_ATTACK         = 0x00000008,    // 03 Taken melee auto attack

    PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS      = 0x00000010,    // 04 Done attack by Spell that has dmg class melee
    PROC_FLAG_TAKEN_SPELL_MELEE_DMG_CLASS     = 0x00000020,    // 05 Taken attack by Spell that has dmg class melee

    PROC_FLAG_DONE_RANGED_AUTO_ATTACK         = 0x00000040,    // 06 Done ranged auto attack
    PROC_FLAG_TAKEN_RANGED_AUTO_ATTACK        = 0x00000080,    // 07 Taken ranged auto attack

    PROC_FLAG_DONE_SPELL_RANGED_DMG_CLASS     = 0x00000100,    // 08 Done attack by Spell that has dmg class ranged
    PROC_FLAG_TAKEN_SPELL_RANGED_DMG_CLASS    = 0x00000200,    // 09 Taken attack by Spell that has dmg class ranged

    PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS   = 0x00000400,    // 10 Done positive spell that has dmg class none
    PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS  = 0x00000800,    // 11 Taken positive spell that has dmg class none

    PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_NEG   = 0x00001000,    // 12 Done negative spell that has dmg class none
    PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_NEG  = 0x00002000,    // 13 Taken negative spell that has dmg class none

    PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS  = 0x00004000,    // 14 Done positive spell that has dmg class magic
    PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS = 0x00008000,    // 15 Taken positive spell that has dmg class magic

    PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG  = 0x00010000,    // 16 Done negative spell that has dmg class magic
    PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG = 0x00020000,    // 17 Taken negative spell that has dmg class magic

    PROC_FLAG_DONE_PERIODIC                   = 0x00040000,    // 18 Successful do periodic (damage / healing)
    PROC_FLAG_TAKEN_PERIODIC                  = 0x00080000,    // 19 Taken spell periodic (damage / healing)

    PROC_FLAG_TAKEN_DAMAGE                    = 0x00100000,    // 20 Taken any damage
    PROC_FLAG_DONE_TRAP_ACTIVATION            = 0x00200000,    // 21 On trap activation (possibly needs name change to ON_GAMEOBJECT_CAST or USE)

    PROC_FLAG_DONE_MAINHAND_ATTACK            = 0x00400000,    // 22 Done main-hand melee attacks (spell and autoattack)
    PROC_FLAG_DONE_OFFHAND_ATTACK             = 0x00800000,    // 23 Done off-hand melee attacks (spell and autoattack)

    PROC_FLAG_DEATH                           = 0x01000000,    // 24 Died in any way

    // flag masks
    AUTO_ATTACK_PROC_FLAG_MASK                = PROC_FLAG_DONE_MELEE_AUTO_ATTACK | PROC_FLAG_TAKEN_MELEE_AUTO_ATTACK
                                                | PROC_FLAG_DONE_RANGED_AUTO_ATTACK | PROC_FLAG_TAKEN_RANGED_AUTO_ATTACK,

    MELEE_PROC_FLAG_MASK                      = PROC_FLAG_DONE_MELEE_AUTO_ATTACK | PROC_FLAG_TAKEN_MELEE_AUTO_ATTACK
                                                | PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS | PROC_FLAG_TAKEN_SPELL_MELEE_DMG_CLASS
                                                | PROC_FLAG_DONE_MAINHAND_ATTACK | PROC_FLAG_DONE_OFFHAND_ATTACK,

    RANGED_PROC_FLAG_MASK                     = PROC_FLAG_DONE_RANGED_AUTO_ATTACK | PROC_FLAG_TAKEN_RANGED_AUTO_ATTACK
                                                | PROC_FLAG_DONE_SPELL_RANGED_DMG_CLASS | PROC_FLAG_TAKEN_SPELL_RANGED_DMG_CLASS,

    SPELL_PROC_FLAG_MASK                      = PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS | PROC_FLAG_TAKEN_SPELL_MELEE_DMG_CLASS
                                                | PROC_FLAG_DONE_SPELL_RANGED_DMG_CLASS | PROC_FLAG_TAKEN_SPELL_RANGED_DMG_CLASS
                                                | PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS | PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS
                                                | PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_NEG | PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_NEG
                                                | PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS | PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS
                                                | PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG | PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG,

    SPELL_CAST_PROC_FLAG_MASK                  = SPELL_PROC_FLAG_MASK | PROC_FLAG_DONE_TRAP_ACTIVATION | RANGED_PROC_FLAG_MASK,

    PERIODIC_PROC_FLAG_MASK                    = PROC_FLAG_DONE_PERIODIC | PROC_FLAG_TAKEN_PERIODIC,

    DONE_HIT_PROC_FLAG_MASK                    = PROC_FLAG_DONE_MELEE_AUTO_ATTACK | PROC_FLAG_DONE_RANGED_AUTO_ATTACK
                                                | PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS | PROC_FLAG_DONE_SPELL_RANGED_DMG_CLASS
                                                | PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_POS | PROC_FLAG_DONE_SPELL_NONE_DMG_CLASS_NEG
                                                | PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_POS | PROC_FLAG_DONE_SPELL_MAGIC_DMG_CLASS_NEG
                                                | PROC_FLAG_DONE_PERIODIC | PROC_FLAG_DONE_MAINHAND_ATTACK | PROC_FLAG_DONE_OFFHAND_ATTACK,

    TAKEN_HIT_PROC_FLAG_MASK                   = PROC_FLAG_TAKEN_MELEE_AUTO_ATTACK | PROC_FLAG_TAKEN_RANGED_AUTO_ATTACK
                                                | PROC_FLAG_TAKEN_SPELL_MELEE_DMG_CLASS | PROC_FLAG_TAKEN_SPELL_RANGED_DMG_CLASS
                                                | PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_POS | PROC_FLAG_TAKEN_SPELL_NONE_DMG_CLASS_NEG
                                                | PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_POS | PROC_FLAG_TAKEN_SPELL_MAGIC_DMG_CLASS_NEG
                                                | PROC_FLAG_TAKEN_PERIODIC | PROC_FLAG_TAKEN_DAMAGE,

    REQ_SPELL_PHASE_PROC_FLAG_MASK             = SPELL_PROC_FLAG_MASK & DONE_HIT_PROC_FLAG_MASK,
};

#define MELEE_BASED_TRIGGER_MASK (PROC_FLAG_DONE_MELEE_AUTO_ATTACK      | \
                                  PROC_FLAG_TAKEN_MELEE_AUTO_ATTACK     | \
                                  PROC_FLAG_DONE_SPELL_MELEE_DMG_CLASS  | \
                                  PROC_FLAG_TAKEN_SPELL_MELEE_DMG_CLASS | \
                                  PROC_FLAG_DONE_RANGED_AUTO_ATTACK     | \
                                  PROC_FLAG_TAKEN_RANGED_AUTO_ATTACK    | \
                                  PROC_FLAG_DONE_SPELL_RANGED_DMG_CLASS | \
                                  PROC_FLAG_TAKEN_SPELL_RANGED_DMG_CLASS)

enum ProcFlagsExLegacy
{
    PROC_EX_NONE                = 0x0000000,                 // If none can tigger on Hit/Crit only (passive spells MUST defined by SpellFamily flag)
    PROC_EX_NORMAL_HIT          = 0x0000001,                 // If set only from normal hit (only damage spells)
    PROC_EX_CRITICAL_HIT        = 0x0000002,
    PROC_EX_MISS                = 0x0000004,
    PROC_EX_RESIST              = 0x0000008,
    PROC_EX_DODGE               = 0x0000010,
    PROC_EX_PARRY               = 0x0000020,
    PROC_EX_BLOCK               = 0x0000040,
    PROC_EX_EVADE               = 0x0000080,
    PROC_EX_IMMUNE              = 0x0000100,
    PROC_EX_DEFLECT             = 0x0000200,
    PROC_EX_ABSORB              = 0x0000400,
    PROC_EX_REFLECT             = 0x0000800,
    PROC_EX_INTERRUPT           = 0x0001000,                 // Melee hit result can be Interrupt (not used)
    PROC_EX_FULL_BLOCK          = 0x0002000,                 // block all attack damage
    PROC_EX_RESERVED2           = 0x0004000,
    PROC_EX_NOT_ACTIVE_SPELL    = 0x0008000,                 // Spell mustn't do damage/heal to proc
    PROC_EX_EX_TRIGGER_ALWAYS   = 0x0010000,                 // If set trigger always no matter of hit result
    PROC_EX_EX_ONE_TIME_TRIGGER = 0x0020000,                 // If set trigger always but only one time (not implemented yet)
    PROC_EX_ONLY_ACTIVE_SPELL   = 0x0040000,                 // Spell has to do damage/heal to proc
    PROC_EX_NO_OVERHEAL         = 0x0080000,                 // Proc if heal did some work
    PROC_EX_NO_AURA_REFRESH     = 0x0100000,                 // Proc if aura was not refreshed
    PROC_EX_ONLY_FIRST_TICK     = 0x0200000,                 // Proc only on first tick (in case of periodic spells)

    // Flags for internal use - do not use these in db!
    PROC_EX_INTERNAL_CANT_PROC  = 0x0800000,
    PROC_EX_INTERNAL_DOT        = 0x1000000,
    PROC_EX_INTERNAL_HOT        = 0x2000000,
    PROC_EX_INTERNAL_TRIGGERED  = 0x4000000,
    PROC_EX_INTERNAL_REQ_FAMILY = 0x8000000
};

#define AURA_SPELL_PROC_EX_MASK \
   (PROC_EX_NORMAL_HIT | PROC_EX_CRITICAL_HIT | PROC_EX_MISS | \
    PROC_EX_RESIST | PROC_EX_DODGE | PROC_EX_PARRY | PROC_EX_BLOCK | \
    PROC_EX_EVADE | PROC_EX_IMMUNE | PROC_EX_DEFLECT | \
    PROC_EX_ABSORB | PROC_EX_REFLECT | PROC_EX_INTERRUPT)

enum ProcFlagsSpellType
{
    PROC_SPELL_TYPE_NONE              = 0x0000000,
    PROC_SPELL_TYPE_DAMAGE            = 0x0000001, // damage type of spell
    PROC_SPELL_TYPE_HEAL              = 0x0000002, // heal type of spell
    PROC_SPELL_TYPE_NO_DMG_HEAL       = 0x0000004, // other spells
    PROC_SPELL_TYPE_MASK_ALL          = PROC_SPELL_TYPE_DAMAGE | PROC_SPELL_TYPE_HEAL | PROC_SPELL_TYPE_NO_DMG_HEAL
};

enum ProcFlagsSpellPhase
{
    PROC_SPELL_PHASE_NONE             = 0x0000000,
    PROC_SPELL_PHASE_CAST             = 0x0000001,
    PROC_SPELL_PHASE_HIT              = 0x0000002,
    PROC_SPELL_PHASE_FINISH           = 0x0000004,
    PROC_SPELL_PHASE_MASK_ALL         = PROC_SPELL_PHASE_CAST | PROC_SPELL_PHASE_HIT | PROC_SPELL_PHASE_FINISH
};

enum ProcFlagsHit
{
    PROC_HIT_NONE                = 0x0000000, // no value - PROC_HIT_NORMAL | PROC_HIT_CRITICAL for TAKEN proc type, PROC_HIT_NORMAL | PROC_HIT_CRITICAL | PROC_HIT_ABSORB for DONE
    PROC_HIT_NORMAL              = 0x0000001, // non-critical hits
    PROC_HIT_CRITICAL            = 0x0000002,
    PROC_HIT_MISS                = 0x0000004,
    PROC_HIT_FULL_RESIST         = 0x0000008,
    PROC_HIT_DODGE               = 0x0000010,
    PROC_HIT_PARRY               = 0x0000020,
    PROC_HIT_BLOCK               = 0x0000040, // partial or full block
    PROC_HIT_EVADE               = 0x0000080,
    PROC_HIT_IMMUNE              = 0x0000100,
    PROC_HIT_DEFLECT             = 0x0000200,
    PROC_HIT_ABSORB              = 0x0000400, // partial or full absorb
    PROC_HIT_REFLECT             = 0x0000800,
    PROC_HIT_INTERRUPT           = 0x0001000, // (not used atm)
    PROC_HIT_FULL_BLOCK          = 0x0002000,
    PROC_HIT_MASK_ALL            = 0x0002FFF,
};

enum ProcAttributes
{
    PROC_ATTR_REQ_EXP_OR_HONOR   = 0x0000010,
};

struct SpellProcEventEntry
{
    uint32      schoolMask;                                 // if nonzero - bit mask for matching proc condition based on spell candidate's school: Fire=2, Mask=1<<(2-1)=2
    uint32      spellFamilyName;                            // if nonzero - for matching proc condition based on candidate spell's SpellFamilyNamer value
    flag96      spellFamilyMask;                            // if nonzero - for matching proc condition based on candidate spell's SpellFamilyFlags  (like auras 107 and 108 do)
    uint32      procFlags;                                  // bitmask for matching proc event
    uint32      procEx;                                     // proc Extend info (see ProcFlagsEx)
    uint32      procPhase;                                  // proc phase (see ProcFlagsSpellPhase)
    float       ppmRate;                                    // for melee (ranged?) damage spells - proc rate per minute. if zero, falls back to flat chance from Spell.dbc
    float       customChance;                               // Owerride chance (in most cases for debug only)
    uint32      cooldown;                                   // hidden cooldown used for some spell proc events, applied to _triggered_spell_
};

using SpellProcEventMap = std::unordered_map<uint32, SpellProcEventEntry>;
struct SpellProcEntry
{
    uint32       SchoolMask;                                 // if nonzero - bitmask for matching proc condition based on spell's school
    uint32       SpellFamilyName;                            // if nonzero - for matching proc condition based on candidate spell's SpellFamilyName
    flag96       SpellFamilyMask;                            // if nonzero - bitmask for matching proc condition based on candidate spell's SpellFamilyFlags
    uint32       ProcFlags;                                  // if nonzero - owerwrite procFlags field for given Spell.dbc entry, bitmask for matching proc condition, see enum ProcFlags
    uint32       SpellTypeMask;                              // if nonzero - bitmask for matching proc condition based on candidate spell's damage/heal effects, see enum ProcFlagsSpellType
    uint32       SpellPhaseMask;                             // if nonzero - bitmask for matching phase of a spellcast on which proc occurs, see enum ProcFlagsSpellPhase
    uint32       HitMask;                                    // if nonzero - bitmask for matching proc condition based on hit result, see enum ProcFlagsHit
    uint32       AttributesMask;                             // bitmask, see ProcAttributes
    float        ProcsPerMinute;                             // if nonzero - chance to proc is equal to value * aura caster's weapon speed / 60
    float        Chance;                                     // if nonzero - owerwrite procChance field for given Spell.dbc entry, defines chance of proc to occur, not used if ProcsPerMinute set
    Milliseconds Cooldown;                                   // if nonzero - cooldown in secs for aura proc, applied to aura
    uint32       Charges;                                    // if nonzero - owerwrite procCharges field for given Spell.dbc entry, defines how many times proc can occur before aura remove, 0 - infinite
};

using SpellProcMap = std::unordered_map<uint32, SpellProcEntry>;
enum EnchantProcAttributes
{
    ENCHANT_PROC_ATTR_EXCLUSIVE     = 0x1, // Only one instance of that effect can be active
    ENCHANT_PROC_ATTR_WHITE_HIT     = 0x2  // Enchant shall only proc off white hits (not abilities)
};

struct SpellEnchantProcEntry
{
    uint32      customChance;
    float       PPMChance;
    uint32      procEx;
    uint32      attributeMask;
};

using SpellEnchantProcEventMap = std::unordered_map<uint32, SpellEnchantProcEntry>;
struct SpellBonusEntry
{
    float  direct_damage;
    float  dot_damage;
    float  ap_bonus;
    float  ap_dot_bonus;
};

using SpellBonusMap = std::unordered_map<uint32, SpellBonusEntry>;
enum SpellGroupSpecialFlags
{
    SPELL_GROUP_SPECIAL_FLAG_NONE                       = 0x000,
    SPELL_GROUP_SPECIAL_FLAG_ELIXIR_BATTLE              = 0x001,
    SPELL_GROUP_SPECIAL_FLAG_ELIXIR_GUARDIAN            = 0x002,
    SPELL_GROUP_SPECIAL_FLAG_ELIXIR_UNSTABLE            = 0x004,
    SPELL_GROUP_SPECIAL_FLAG_ELIXIR_SHATTRATH           = 0x008,
    SPELL_GROUP_SPECIAL_FLAG_STACK_EXCLUSIVE_MAX        = 0x00F,
    SPELL_GROUP_SPECIAL_FLAG_FORCED_STRONGEST           = 0x010, // xinef: specially helpful flag if some spells have different auras, but only one should be present
    SPELL_GROUP_SPECIAL_FLAG_SKIP_STRONGER_CHECK        = 0x020,
    SPELL_GROUP_SPECIAL_FLAG_BASE_AMOUNT_CHECK          = 0x040,
    SPELL_GROUP_SPECIAL_FLAG_PRIORITY1                  = 0x100,
    SPELL_GROUP_SPECIAL_FLAG_PRIORITY2                  = 0x200,
    SPELL_GROUP_SPECIAL_FLAG_PRIORITY3                  = 0x400,
    SPELL_GROUP_SPECIAL_FLAG_PRIORITY4                  = 0x800,
    SPELL_GROUP_SPECIAL_FLAG_SAME_SPELL_CHECK           = 0x1000,
    SPELL_GROUP_SPECIAL_FLAG_SKIP_STRONGER_SAME_SPELL   = 0x2000,
    SPELL_GROUP_SPECIAL_FLAG_MAX                        = 0x4000,

    SPELL_GROUP_SPECIAL_FLAG_FLASK                      = SPELL_GROUP_SPECIAL_FLAG_ELIXIR_BATTLE | SPELL_GROUP_SPECIAL_FLAG_ELIXIR_GUARDIAN
};

enum SpellGroupStackFlags
{
    SPELL_GROUP_STACK_FLAG_NONE                 = 0x00,
    SPELL_GROUP_STACK_FLAG_EXCLUSIVE            = 0x01,
    SPELL_GROUP_STACK_FLAG_NOT_SAME_CASTER      = 0x02,
    SPELL_GROUP_STACK_FLAG_FLAGGED              = 0x04, // xinef: just a marker
    SPELL_GROUP_STACK_FLAG_NEVER_STACK          = 0x08,
    SPELL_GROUP_STACK_FLAG_EFFECT_EXCLUSIVE     = 0x10,
    SPELL_GROUP_STACK_FLAG_MAX                  = 0x20,

    // Internal use
    SPELL_GROUP_STACK_FLAG_FORCED_STRONGEST     = 0x100,
    SPELL_GROUP_STACK_FLAG_FORCED_WEAKEST       = 0x200,
};

enum SpellGroupIDs
{
    SPELL_GROUP_GUARDIAN_AND_BATTLE_ELIXIRS     = 1
};

struct SpellStackInfo
{
    uint32 groupId;
    SpellGroupSpecialFlags specialFlags;
};
//             spell_id, group_id
using SpellGroupMap = std::map<uint32, SpellStackInfo>;
using SpellGroupStackMap = std::map<uint32, SpellGroupStackFlags>;
struct SpellThreatEntry
{
    int32       flatMod;                                    // flat threat-value for this Spell  - default: 0
    float       pctMod;                                     // threat-multiplier for this Spell  - default: 1.0f
    float       apPctMod;                                   // Pct of AP that is added as Threat - default: 0.0f
};

using SpellThreatMap = std::unordered_map<uint32, SpellThreatEntry>;
using SpellMixologyMap = std::map<uint32, float>;
// coordinates for spells (accessed using SpellMgr functions)
struct SpellTargetPosition
{
    uint32 target_mapId;
    float  target_X;
    float  target_Y;
    float  target_Z;
    float  target_Orientation;
};

using SpellTargetPositionMap = std::map<std::pair<uint32 /*spell_id*/, SpellEffIndex /*effIndex*/>, SpellTargetPosition>;
// Enum with EffectRadiusIndex and their actual radius
enum EffectRadiusIndex
{
    EFFECT_RADIUS_2_YARDS       = 7,
    EFFECT_RADIUS_5_YARDS       = 8,
    EFFECT_RADIUS_20_YARDS      = 9,
    EFFECT_RADIUS_30_YARDS      = 10,
    EFFECT_RADIUS_45_YARDS      = 11,
    EFFECT_RADIUS_100_YARDS     = 12,
    EFFECT_RADIUS_10_YARDS      = 13,
    EFFECT_RADIUS_8_YARDS       = 14,
    EFFECT_RADIUS_3_YARDS       = 15,
    EFFECT_RADIUS_1_YARD        = 16,
    EFFECT_RADIUS_13_YARDS      = 17,
    EFFECT_RADIUS_15_YARDS      = 18,
    EFFECT_RADIUS_18_YARDS      = 19,
    EFFECT_RADIUS_25_YARDS      = 20,
    EFFECT_RADIUS_35_YARDS      = 21,
    EFFECT_RADIUS_200_YARDS     = 22,
    EFFECT_RADIUS_40_YARDS      = 23,
    EFFECT_RADIUS_65_YARDS      = 24,
    EFFECT_RADIUS_70_YARDS      = 25,
    EFFECT_RADIUS_4_YARDS       = 26,
    EFFECT_RADIUS_50_YARDS      = 27,
    EFFECT_RADIUS_50000_YARDS   = 28,
    EFFECT_RADIUS_6_YARDS       = 29,
    EFFECT_RADIUS_500_YARDS     = 30,
    EFFECT_RADIUS_80_YARDS      = 31,
    EFFECT_RADIUS_12_YARDS      = 32,
    EFFECT_RADIUS_99_YARDS      = 33,
    EFFECT_RADIUS_55_YARDS      = 35,
    EFFECT_RADIUS_0_YARDS       = 36,
    EFFECT_RADIUS_7_YARDS       = 37,
    EFFECT_RADIUS_21_YARDS      = 38,
    EFFECT_RADIUS_34_YARDS      = 39,
    EFFECT_RADIUS_9_YARDS       = 40,
    EFFECT_RADIUS_150_YARDS     = 41,
    EFFECT_RADIUS_11_YARDS      = 42,
    EFFECT_RADIUS_16_YARDS      = 43,
    EFFECT_RADIUS_0_5_YARDS     = 44,   // 0.5 yards
    EFFECT_RADIUS_10_YARDS_2    = 45,
    EFFECT_RADIUS_5_YARDS_2     = 46,
    EFFECT_RADIUS_15_YARDS_2    = 47,
    EFFECT_RADIUS_60_YARDS      = 48,
    EFFECT_RADIUS_90_YARDS      = 49,
    EFFECT_RADIUS_15_YARDS_3    = 50,
    EFFECT_RADIUS_60_YARDS_2    = 51,
    EFFECT_RADIUS_5_YARDS_3     = 52,
    EFFECT_RADIUS_60_YARDS_3    = 53,
    EFFECT_RADIUS_50000_YARDS_2 = 54,
    EFFECT_RADIUS_130_YARDS     = 55,
    EFFECT_RADIUS_38_YARDS      = 56,
    EFFECT_RADIUS_45_YARDS_2    = 57,
    EFFECT_RADIUS_32_YARDS      = 59,
    EFFECT_RADIUS_44_YARDS      = 60,
    EFFECT_RADIUS_14_YARDS      = 61,
    EFFECT_RADIUS_47_YARDS      = 62,
    EFFECT_RADIUS_23_YARDS      = 63,
    EFFECT_RADIUS_3_5_YARDS     = 64,   // 3.5 yards
    EFFECT_RADIUS_80_YARDS_2    = 65
};

// Spell pet auras
class PetAura
{
private:
    using PetAuraMap = std::unordered_map<uint32, uint32>;
public:
    PetAura()
    {
        auras.clear();
    }

    PetAura(uint32 petEntry, uint32 aura, bool _removeOnChangePet, int _damage) :
        removeOnChangePet(_removeOnChangePet), damage(_damage)
    {
        auras[petEntry] = aura;
    }

    [[nodiscard]] uint32 GetAura(uint32 petEntry) const
    {
        PetAuraMap::const_iterator itr = auras.find(petEntry);
        if (itr != auras.end())
            return itr->second;
        PetAuraMap::const_iterator itr2 = auras.find(0);
        if (itr2 != auras.end())
            return itr2->second;
        return 0;
    }

    void AddAura(uint32 petEntry, uint32 aura)
    {
        auras[petEntry] = aura;
    }

    [[nodiscard]] bool IsRemovedOnChangePet() const
    {
        return removeOnChangePet;
    }

    [[nodiscard]] int32 GetDamage() const
    {
        return damage;
    }

private:
    PetAuraMap auras;
    bool removeOnChangePet{false};
    int32 damage{0};
};
using SpellPetAuraMap = std::map<uint32, PetAura>;
enum ICCBuff
{
    ICC_AREA              = 4812,
    ICC_RACEMASK_HORDE    =  690,
    ICC_RACEMASK_ALLIANCE = 1101
};

struct SpellArea
{
    uint32 spellId;
    uint32 areaId;                                          // zone/subzone/or 0 is not limited to zone
    uint32 questStart;                                      // quest start (quest must be active or rewarded for spell apply)
    uint32 questEnd;                                        // quest end (quest must not be rewarded for spell apply)
    int32  auraSpell;                                       // spell aura must be applied for spell apply)if possitive) and it must not be applied in other case
    uint32 raceMask;                                        // can be applied only to races
    Gender gender;                                          // can be applied only to gender
    uint32 questStartStatus;                                // QuestStatus that quest_start must have in order to keep the spell
    uint32 questEndStatus;                                  // QuestStatus that the quest_end must have in order to keep the spell (if the quest_end's status is different than this, the spell will be dropped)
    bool autocast;                                          // if true then auto applied at area enter, in other case just allowed to cast

    // helpers
    bool IsFitToRequirements(Player const* player, uint32 newZone, uint32 newArea) const;
};

using SpellAreaMap = std::multimap<uint32, SpellArea>;
using SpellAreaForQuestMap = std::multimap<uint32, SpellArea const*>;
using SpellAreaForAuraMap = std::multimap<uint32, SpellArea const*>;
using SpellAreaForAreaMap = std::multimap<uint32, SpellArea const*>;
using SpellAreaMapBounds = std::pair<SpellAreaMap::const_iterator, SpellAreaMap::const_iterator>;
using SpellAreaForQuestMapBounds = std::pair<SpellAreaForQuestMap::const_iterator, SpellAreaForQuestMap::const_iterator>;
using SpellAreaForAuraMapBounds = std::pair<SpellAreaForAuraMap::const_iterator, SpellAreaForAuraMap::const_iterator>;
using SpellAreaForAreaMapBounds = std::pair<SpellAreaForAreaMap::const_iterator, SpellAreaForAreaMap::const_iterator>;
// Spell rank chain  (accessed using SpellMgr functions)
struct SpellChainNode
{
    SpellInfo const* prev;
    SpellInfo const* next;
    SpellInfo const* first;
    SpellInfo const* last;
    uint8  rank;
};

using SpellChainMap = std::unordered_map<uint32, SpellChainNode>;
//                   spell_id  req_spell
using SpellRequiredMap = std::multimap<uint32, uint32>;
using SpellRequiredMapBounds = std::pair<SpellRequiredMap::const_iterator, SpellRequiredMap::const_iterator>;
//                   req_spell spell_id
using SpellsRequiringSpellMap = std::multimap<uint32, uint32>;
using SpellsRequiringSpellMapBounds = std::pair<SpellsRequiringSpellMap::const_iterator, SpellsRequiringSpellMap::const_iterator>;
// Spell learning properties (accessed using SpellMgr functions)
struct SpellLearnSkillNode
{
    uint16 skill;
    uint16 step;
    uint16 value;                                           // 0  - max skill value for player level
    uint16 maxvalue;                                        // 0  - max skill value for player level
};

using SpellLearnSkillMap = std::unordered_map<uint32, SpellLearnSkillNode>;
using SkillLineAbilityMap = std::multimap<uint32, SkillLineAbilityEntry const*>;
using SkillLineAbilityMapBounds = std::pair<SkillLineAbilityMap::const_iterator, SkillLineAbilityMap::const_iterator>;
using PetLevelupSpellSet = std::multimap<uint32, uint32>;
using PetLevelupSpellMap = std::map<uint32, PetLevelupSpellSet>;
using SpellDifficultySearcherMap = std::map<uint32, uint32>;
struct PetDefaultSpellsEntry
{
    uint32 spellid[MAX_CREATURE_SPELL_DATA_SLOT];
};

// < 0 for petspelldata id, > 0 for creature_id
using PetDefaultSpellsMap = std::map<int32, PetDefaultSpellsEntry>;
using SpellCustomAttribute = std::vector<uint32>;
using EnchantCustomAttribute = std::vector<bool>;
using SpellInfoMap = std::vector<SpellInfo*>;
using SpellLinkedMap = std::map<int32, std::vector<int32> >;
struct SpellCooldownOverride
{
    uint32 RecoveryTime;
    uint32 CategoryRecoveryTime;
    uint32 StartRecoveryTime;
    uint32 StartRecoveryCategory;
};

using SpellCooldownOverrideMap = std::map<uint32, SpellCooldownOverride>;
bool IsPrimaryProfessionSkill(uint32 skill);

inline bool IsProfessionSkill(uint32 skill)
{
    return  IsPrimaryProfessionSkill(skill) || skill == SKILL_FISHING || skill == SKILL_COOKING || skill == SKILL_FIRST_AID;
}

inline bool IsProfessionOrRidingSkill(uint32 skill)
{
    return  IsProfessionSkill(skill) || skill == SKILL_RIDING;
}

bool IsPartOfSkillLine(uint32 skillId, uint32 spellId);

// spell diminishing returns
DiminishingGroup GetDiminishingReturnsGroupForSpell(SpellInfo const* spellproto, bool triggered);
DiminishingReturnsType GetDiminishingReturnsGroupType(DiminishingGroup group);
DiminishingLevels GetDiminishingReturnsMaxLevel(DiminishingGroup group);
int32 GetDiminishingReturnsLimitDuration(DiminishingGroup group, SpellInfo const* spellproto);
bool IsDiminishingReturnsGroupDurationLimited(DiminishingGroup group);

using TalentAdditionalSet = std::set<uint32>;
class SpellMgr
{
    // Constructors
private:
    SpellMgr();
    ~SpellMgr();

    // Accessors (const or static functions)
public:
    static SpellMgr* instance();

    // Spell correctness for client using
    static bool ComputeIsSpellValid(SpellInfo const* spellInfo, bool msg = true);
    static bool IsSpellValid(SpellInfo const* spellInfo);
    static bool CheckSpellValid(SpellInfo const* spellInfo, uint32 spellId, bool isTalent);

    // Spell difficulty
    [[nodiscard]] uint32 GetSpellDifficultyId(uint32 spellId) const;
    void SetSpellDifficultyId(uint32 spellId, uint32 id);
    uint32 GetSpellIdForDifficulty(uint32 spellId, Unit const* caster) const;
    SpellInfo const* GetSpellForDifficultyFromSpell(SpellInfo const* spell, Unit const* caster) const;

    // Spell Ranks table
    [[nodiscard]] SpellChainNode const* GetSpellChainNode(uint32 spell_id) const;
    [[nodiscard]] uint32 GetFirstSpellInChain(uint32 spell_id) const;
    [[nodiscard]] uint32 GetLastSpellInChain(uint32 spell_id) const;
    [[nodiscard]] uint32 GetNextSpellInChain(uint32 spell_id) const;
    [[nodiscard]] uint32 GetPrevSpellInChain(uint32 spell_id) const;
    [[nodiscard]] uint8 GetSpellRank(uint32 spell_id) const;
    // not strict check returns provided spell if rank not avalible
    [[nodiscard]] uint32 GetSpellWithRank(uint32 spell_id, uint32 rank, bool strict = false) const;

    // Spell Required table
    [[nodiscard]] SpellRequiredMapBounds GetSpellsRequiredForSpellBounds(uint32 spell_id) const;
    [[nodiscard]] SpellsRequiringSpellMapBounds GetSpellsRequiringSpellBounds(uint32 spell_id) const;
    [[nodiscard]] bool IsSpellRequiringSpell(uint32 spellid, uint32 req_spellid) const;

    // Spell learning
    [[nodiscard]] SpellLearnSkillNode const* GetSpellLearnSkill(uint32 spell_id) const;

    // Spell target coordinates
    [[nodiscard]] SpellTargetPosition const* GetSpellTargetPosition(uint32 spell_id, SpellEffIndex effIndex) const;

    // Spell Groups
    [[nodiscard]] uint32 GetSpellGroup(uint32 spellid) const;
    [[nodiscard]] SpellGroupSpecialFlags GetSpellGroupSpecialFlags(uint32 spell_id) const;
    [[nodiscard]] SpellGroupStackFlags GetGroupStackFlags(uint32 groupid) const;
    SpellGroupStackFlags CheckSpellGroupStackRules(SpellInfo const* spellInfo1, SpellInfo const* spellInfo2, bool remove, bool areaAura) const;
    void GetSetOfSpellsInSpellGroupWithFlag(uint32 group_id, SpellGroupSpecialFlags flag, std::set<uint32>& availableElixirs) const;

    // Spell proc event table
    [[nodiscard]] SpellProcEventEntry const* GetSpellProcEvent(uint32 spellId) const;
    bool IsSpellProcEventCanTriggeredBy(SpellInfo const* spellProto, SpellProcEventEntry const* spellProcEvent, uint32 EventProcFlag, ProcEventInfo const& eventInfo, bool active) const;

    // Spell proc table
    [[nodiscard]] SpellProcEntry const* GetSpellProcEntry(uint32 spellId) const;
    bool CanSpellTriggerProcOnEvent(SpellProcEntry const& procEntry, ProcEventInfo& eventInfo) const;

    // Spell bonus data table
    [[nodiscard]] SpellBonusEntry const* GetSpellBonusData(uint32 spellId) const;

    // Spell threat table
    [[nodiscard]] SpellThreatEntry const* GetSpellThreatEntry(uint32 spellID) const;

    // Spell mixology table
    [[nodiscard]] float GetSpellMixologyBonus(uint32 spellId) const;

    [[nodiscard]] SkillLineAbilityMapBounds GetSkillLineAbilityMapBounds(uint32 spell_id) const;

    [[nodiscard]] PetAura const* GetPetAura(uint32 spell_id, uint8 eff) const;

    [[nodiscard]] SpellEnchantProcEntry const* GetSpellEnchantProcEvent(uint32 enchId) const;
    [[nodiscard]] bool IsArenaAllowedEnchancment(uint32 ench_id) const;

    [[nodiscard]] const std::vector<int32>* GetSpellLinked(int32 spell_id) const;

    [[nodiscard]] PetLevelupSpellSet const* GetPetLevelupSpellList(uint32 petFamily) const;
    [[nodiscard]] PetDefaultSpellsEntry const* GetPetDefaultSpellsEntry(int32 id) const;

    // Spell area
    [[nodiscard]] SpellAreaMapBounds GetSpellAreaMapBounds(uint32 spell_id) const;
    [[nodiscard]] SpellAreaForQuestMapBounds GetSpellAreaForQuestMapBounds(uint32 quest_id) const;
    [[nodiscard]] SpellAreaForQuestMapBounds GetSpellAreaForQuestEndMapBounds(uint32 quest_id) const;
    [[nodiscard]] SpellAreaForAuraMapBounds GetSpellAreaForAuraMapBounds(uint32 spell_id) const;
    [[nodiscard]] SpellAreaForAreaMapBounds GetSpellAreaForAreaMapBounds(uint32 area_id) const;

    // SpellInfo object management
    [[nodiscard]] SpellInfo const* GetSpellInfo(uint32 spellId) const { return spellId < GetSpellInfoStoreSize() ?  mSpellInfoMap[spellId] : nullptr; }
    // Use this only with 100% valid spellIds
    [[nodiscard]] SpellInfo const* AssertSpellInfo(uint32 spellId) const
    {
        ASSERT(spellId < GetSpellInfoStoreSize());
        SpellInfo const* spellInfo = mSpellInfoMap[spellId];
        ASSERT(spellInfo);
        return spellInfo;
    }
    // use this instead of AssertSpellInfo to have the problem logged instead of crashing the server
    [[nodiscard]] SpellInfo const* CheckSpellInfo(uint32 spellId) const
    {
        if (spellId >= GetSpellInfoStoreSize())
        {
            LOG_ERROR("server", "spellId {} is not lower than GetSpellInfoStoreSize() ({})", spellId, GetSpellInfoStoreSize());
            return nullptr;
        }
        SpellInfo const* spellInfo = mSpellInfoMap[spellId];
        if (!spellInfo)
        {
            LOG_ERROR("server", "spellId {} has invalid spellInfo", spellId);
            return nullptr;
        }
        return spellInfo;
    }
    [[nodiscard]] uint32 GetSpellInfoStoreSize() const { return mSpellInfoMap.size(); }

    // Talent Additional Set
    [[nodiscard]] bool IsAdditionalTalentSpell(uint32 spellId) const;

    [[nodiscard]] bool HasSpellCooldownOverride(uint32 spellId) const;
    [[nodiscard]] SpellCooldownOverride GetSpellCooldownOverride(uint32 spellId) const;

private:
    SpellInfo* _GetSpellInfo(uint32 spellId) { return spellId < GetSpellInfoStoreSize() ? mSpellInfoMap[spellId] : nullptr; }

    // Modifiers
public:
    // Loading data at server startup
    void UnloadSpellInfoChains();
    void LoadSpellTalentRanks();
    void LoadSpellRanks();
    void LoadSpellRequired();
    void LoadSpellLearnSkills();
    void LoadSpellTargetPositions();
    void LoadSpellGroups();
    void LoadSpellGroupStackRules();
    void LoadSpellProcEvents();
    void LoadSpellProcs();
    void LoadSpellBonuses();
    void LoadSpellThreats();
    void LoadSpellMixology();
    void LoadSkillLineAbilityMap();
    void LoadSpellPetAuras();
    void LoadEnchantCustomAttr();
    void LoadSpellEnchantProcData();
    void LoadSpellLinked();
    void LoadPetLevelupSpellMap();
    void LoadPetDefaultSpells();
    void LoadSpellAreas();
    void LoadSpellInfoStore();
    void LoadSpellCooldownOverrides();
    void UnloadSpellInfoStore();
    void UnloadSpellInfoImplicitTargetConditionLists();
    void LoadSpellInfoCustomAttributes();
    void LoadSpellInfoCorrections();
    void LoadSpellSpecificAndAuraState();

private:
    SpellDifficultySearcherMap mSpellDifficultySearcherMap;
    SpellChainMap              mSpellChains;
    SpellsRequiringSpellMap    mSpellsReqSpell;
    SpellRequiredMap           mSpellReq;
    SpellLearnSkillMap         mSpellLearnSkills;
    SpellTargetPositionMap     mSpellTargetPositions;
    SpellGroupMap              mSpellGroupMap;
    SpellGroupStackMap         mSpellGroupStackMap;
    SpellProcEventMap          mSpellProcEventMap;
    SpellProcMap               mSpellProcMap;
    SpellBonusMap              mSpellBonusMap;
    SpellThreatMap             mSpellThreatMap;
    SpellMixologyMap           mSpellMixologyMap;
    SpellPetAuraMap            mSpellPetAuraMap;
    SpellLinkedMap             mSpellLinkedMap;
    SpellEnchantProcEventMap   mSpellEnchantProcEventMap;
    EnchantCustomAttribute     mEnchantCustomAttr;
    SpellAreaMap               mSpellAreaMap;
    SpellAreaForQuestMap       mSpellAreaForQuestMap;
    SpellAreaForQuestMap       mSpellAreaForQuestEndMap;
    SpellAreaForAuraMap        mSpellAreaForAuraMap;
    SpellAreaForAreaMap        mSpellAreaForAreaMap;
    SkillLineAbilityMap        mSkillLineAbilityMap;
    PetLevelupSpellMap         mPetLevelupSpellMap;
    PetDefaultSpellsMap        mPetDefaultSpellsMap;           // only spells not listed in related mPetLevelupSpellMap entry
    SpellInfoMap               mSpellInfoMap;
    SpellCooldownOverrideMap   mSpellCooldownOverrideMap;
    TalentAdditionalSet        mTalentSpellAdditionalSet;
};

#define sSpellMgr SpellMgr::instance()

#endif
