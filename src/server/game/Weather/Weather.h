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

/// \addtogroup world
/// @{
/// \file

#ifndef __WEATHER_H
#define __WEATHER_H

#include "SharedDefines.h"
#include "Timer.h"

class Player;

#define WEATHER_SEASONS 4
struct WeatherSeasonChances
{
    uint32 rainChance;
    uint32 snowChance;
    uint32 stormChance;
};

struct WeatherData
{
    WeatherSeasonChances data[WEATHER_SEASONS];
    uint32 ScriptId;
};

enum WeatherState : uint32
{
    WEATHER_STATE_FINE              = 0,
    WEATHER_STATE_FOG               = 1,
    WEATHER_STATE_DRIZZLE           = 2,
    WEATHER_STATE_LIGHT_RAIN        = 3,
    WEATHER_STATE_MEDIUM_RAIN       = 4,
    WEATHER_STATE_HEAVY_RAIN        = 5,
    WEATHER_STATE_LIGHT_SNOW        = 6,
    WEATHER_STATE_MEDIUM_SNOW       = 7,
    WEATHER_STATE_HEAVY_SNOW        = 8,
    WEATHER_STATE_LIGHT_SANDSTORM   = 22,
    WEATHER_STATE_MEDIUM_SANDSTORM  = 41,
    WEATHER_STATE_HEAVY_SANDSTORM   = 42,
    WEATHER_STATE_THUNDERS          = 86,
    WEATHER_STATE_BLACKRAIN         = 90,
    WEATHER_STATE_BLACKSNOW         = 106
};

/// Weather for one zone
class Weather
{
public:
    Weather(uint32 zoneID, WeatherData const* weatherChances);
    ~Weather() = default;

    bool Update(uint32 diff);
    bool ReGenerate();
    bool UpdateWeather();

    void SendWeatherUpdateToPlayer(Player* player);
    static void SendFineWeatherUpdateToPlayer(Player* player);
    void SetWeather(WeatherType type, float intensity);

    [[nodiscard]] uint32 GetZone() const { return _zone; };
    [[nodiscard]] uint32 GetScriptId() const { return _weatherChances->ScriptId; }

private:
    [[nodiscard]] WeatherState GetWeatherState() const;
    uint32 _zone;
    WeatherType _type;
    float _intensity;
    IntervalTimer _timer;
    WeatherData const* _weatherChances;
};
#endif
