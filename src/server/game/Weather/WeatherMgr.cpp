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

#include "Containers.h"
#include "Log.h"
#include "MiscPackets.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "Timer.h"
#include "Weather.h"
#include "WeatherMgr.h"

namespace WeatherMgr
{

    namespace
    {
        std::unordered_map<uint32, WeatherData> _weatherData;
    }

    WeatherData const* GetWeatherData(uint32 zoneID)
    {
        return Acore::Containers::MapGetValuePtr(_weatherData, zoneID);
    }

    void LoadWeatherData()
    {
        uint32 oldMSTime = getMSTime();

        uint32 count = 0;

        QueryResult result = WorldDatabase.Query("SELECT "
                             "zone, spring_rain_chance, spring_snow_chance, spring_storm_chance,"
                             "summer_rain_chance, summer_snow_chance, summer_storm_chance,"
                             "fall_rain_chance, fall_snow_chance, fall_storm_chance,"
                             "winter_rain_chance, winter_snow_chance, winter_storm_chance,"
                             "ScriptName FROM game_weather");

        if (!result)
        {
            LOG_WARN("server.loading", ">> Loaded 0 weather definitions. DB table `game_weather` is empty.");
            LOG_INFO("server.loading", " ");
            return;
        }

        do
        {
            Field* fields = result->Fetch();

            uint32 zoneID = fields[0].Get<uint32>();

            WeatherData& wzc = _weatherData[zoneID];

            for (uint8 season = 0; season < WEATHER_SEASONS; ++season)
            {
                wzc.data[season].rainChance  = fields[season * (MAX_WEATHER_TYPE - 1) + 1].Get<uint8>();
                wzc.data[season].snowChance  = fields[season * (MAX_WEATHER_TYPE - 1) + 2].Get<uint8>();
                wzc.data[season].stormChance = fields[season * (MAX_WEATHER_TYPE - 1) + 3].Get<uint8>();

                if (wzc.data[season].rainChance > 100)
                {
                    wzc.data[season].rainChance = 25;
                    LOG_ERROR("sql.sql", "Weather for zone {} season {} has wrong rain chance > 100%", zoneID, season);
                }

                if (wzc.data[season].snowChance > 100)
                {
                    wzc.data[season].snowChance = 25;
                    LOG_ERROR("sql.sql", "Weather for zone {} season {} has wrong snow chance > 100%", zoneID, season);
                }

                if (wzc.data[season].stormChance > 100)
                {
                    wzc.data[season].stormChance = 25;
                    LOG_ERROR("sql.sql", "Weather for zone {} season {} has wrong storm chance > 100%", zoneID, season);
                }
            }

            wzc.ScriptId = sObjectMgr->GetScriptId(fields[13].Get<std::string>());

            ++count;
        } while (result->NextRow());

        LOG_INFO("server.loading", ">> Loaded {} Weather Definitions in {} ms", count, GetMSTimeDiffToNow(oldMSTime));
        LOG_INFO("server.loading", " ");
    }

} // namespace
