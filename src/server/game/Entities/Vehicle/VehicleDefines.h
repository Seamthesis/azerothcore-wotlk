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

#ifndef __ACORE_VEHICLEDEFINES_H
#define __ACORE_VEHICLEDEFINES_H

#include "Define.h"
#include "Map.h"
#include <map>
#include <vector>

struct VehicleSeatEntry;

enum PowerType
{
    POWER_STEAM                                  = 61,
    POWER_PYRITE                                 = 41,
    POWER_HEAT                                   = 101,
    POWER_OOZE                                   = 121,
    POWER_BLOOD                                  = 141,
    POWER_WRATH                                  = 142
};

enum VehicleFlags
{
    VEHICLE_FLAG_NO_STRAFE                       = 0x00000001,           // Sets MOVEFLAG2_NO_STRAFE
    VEHICLE_FLAG_NO_JUMPING                      = 0x00000002,           // Sets MOVEFLAG2_NO_JUMPING
    VEHICLE_FLAG_FULLSPEEDTURNING                = 0x00000004,           // Sets MOVEFLAG2_FULLSPEEDTURNING
    VEHICLE_FLAG_ALLOW_PITCHING                  = 0x00000010,           // Sets MOVEFLAG2_ALLOW_PITCHING
    VEHICLE_FLAG_FULLSPEEDPITCHING               = 0x00000020,           // Sets MOVEFLAG2_FULLSPEEDPITCHING
    VEHICLE_FLAG_CUSTOM_PITCH                    = 0x00000040,           // If set use pitchMin and pitchMax from DBC, otherwise pitchMin = -pi/2, pitchMax = pi/2
    VEHICLE_FLAG_ADJUST_AIM_ANGLE                = 0x00000400,           // Lua_IsVehicleAimAngleAdjustable
    VEHICLE_FLAG_ADJUST_AIM_POWER                = 0x00000800,           // Lua_IsVehicleAimPowerAdjustable
};

enum VehicleSpells
{
    VEHICLE_SPELL_RIDE_HARDCODED                 = 46598,
    VEHICLE_SPELL_PARACHUTE                      = 45472,

    VEHICLE_SPELL_GEIST_CONTROL_END              = 58119,
    VEHICLE_SPELL_SHADE_CONTROL_END              = 58664
};

enum class VehicleExitParameters
{
    VehicleExitParamNone    = 0, // provided parameters will be ignored
    VehicleExitParamOffset  = 1, // provided parameters will be used as offset values
    VehicleExitParamDest    = 2, // provided parameters will be used as absolute destination
    VehicleExitParamMax
};

enum VehicleNPCs
{
    NPC_EIDOLON_WATCHER                         = 31110,
    NPC_LITHE_STALKER                           = 30895
};

struct PassengerInfo
{
    ObjectGuid Guid;
    bool IsUnselectable;

    void Reset()
    {
        Guid.Clear();
        IsUnselectable = false;
    }
};

struct VehicleSeatAddon
{
    VehicleSeatAddon() { }
    VehicleSeatAddon(float orientatonOffset, float exitX, float exitY, float exitZ, float exitO, uint8 param) :
        SeatOrientationOffset(orientatonOffset), ExitParameterX(exitX), ExitParameterY(exitY), ExitParameterZ(exitZ),
        ExitParameterO(exitO), ExitParameter(VehicleExitParameters(param)) { }

    float SeatOrientationOffset = 0.f;
    float ExitParameterX = 0.f;
    float ExitParameterY = 0.f;
    float ExitParameterZ = 0.f;
    float ExitParameterO = 0.f;
    VehicleExitParameters ExitParameter = VehicleExitParameters::VehicleExitParamNone;
};

struct VehicleSeat
{
    explicit VehicleSeat(VehicleSeatEntry const* seatInfo, VehicleSeatAddon const* seatAddon) : SeatInfo(seatInfo), SeatAddon(seatAddon)
    {
        Passenger.Reset();
    }

    [[nodiscard]] bool IsEmpty() const { return !Passenger.Guid; }

    VehicleSeatEntry const* SeatInfo;
    VehicleSeatAddon const* SeatAddon;
    PassengerInfo Passenger;
};

struct VehicleAccessory
{
    VehicleAccessory(uint32 entry, int8 seatId, bool isMinion, uint8 summonType, uint32 summonTime) :
        AccessoryEntry(entry), IsMinion(isMinion), SummonTime(summonTime), SeatId(seatId), SummonedType(summonType) {}
    uint32 AccessoryEntry;
    uint32 IsMinion;
    uint32 SummonTime;
    int8 SeatId;
    uint8 SummonedType;
};

using VehicleAccessoryList = std::vector<VehicleAccessory>;
using VehicleAccessoryContainer = std::map<uint32, VehicleAccessoryList>;
using SeatMap = std::map<int8, VehicleSeat>;
class TransportBase
{
protected:
    TransportBase() = default;
    virtual ~TransportBase() = default;

public:
    /// This method transforms supplied transport offsets into global coordinates
    virtual void CalculatePassengerPosition(float& x, float& y, float& z, float* o = nullptr) const = 0;

    /// This method transforms supplied global coordinates into local offsets
    virtual void CalculatePassengerOffset(float& x, float& y, float& z, float* o = nullptr) const = 0;

protected:
    static void CalculatePassengerPosition(float& x, float& y, float& z, float* o, float transX, float transY, float transZ, float transO)
    {
        float inx = x, iny = y, inz = z;
        if (o)
            *o = Position::NormalizeOrientation(transO + *o);

        x = transX + inx * std::cos(transO) - iny * std::sin(transO);
        y = transY + iny * std::cos(transO) + inx * std::sin(transO);
        z = transZ + inz;
    }

    static void CalculatePassengerOffset(float& x, float& y, float& z, float* o, float transX, float transY, float transZ, float transO)
    {
        if (o)
            *o = Position::NormalizeOrientation(*o - transO);

        z -= transZ;
        y -= transY;    // y = searchedY * std::cos(o) + searchedX * std::sin(o)
        x -= transX;    // x = searchedX * std::cos(o) + searchedY * std::sin(o + pi)
        float inx = x, iny = y;
        y = (iny - inx * std::tan(transO)) / (std::cos(transO) + std::sin(transO) * std::tan(transO));
        x = (inx + iny * std::tan(transO)) / (std::cos(transO) + std::sin(transO) * std::tan(transO));
    }
};

#endif
