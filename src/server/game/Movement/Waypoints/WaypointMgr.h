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

#ifndef ACORE_WAYPOINTMANAGER_H
#define ACORE_WAYPOINTMANAGER_H

#include "Define.h"
#include <optional>
#include <unordered_map>
#include <vector>

enum WaypointMoveType
{
    WAYPOINT_MOVE_TYPE_WALK,
    WAYPOINT_MOVE_TYPE_RUN,
    WAYPOINT_MOVE_TYPE_LAND,
    WAYPOINT_MOVE_TYPE_TAKEOFF,

    WAYPOINT_MOVE_TYPE_MAX
};

struct WaypointData
{
    uint32 id;
    float x, y, z;
    std::optional<float> orientation;
    uint32 delay;
    uint32 event_id;
    uint32 move_type;
    uint8 event_chance;
};

using WaypointPath = std::vector<WaypointData*>;
using WaypointPathContainer = std::unordered_map<uint32, WaypointPath>;
class WaypointMgr
{
public:
    static WaypointMgr* instance();

    // Attempts to reload a single path from database
    void ReloadPath(uint32 id);

    // Loads all paths from database, should only run on startup
    void Load();

    // Returns the path from a given id
    WaypointPath const* GetPath(uint32 id) const
    {
        WaypointPathContainer::const_iterator itr = _waypointStore.find(id);
        if (itr != _waypointStore.end())
            return &itr->second;

        return nullptr;
    }

private:
    WaypointMgr();
    ~WaypointMgr();

    WaypointPathContainer _waypointStore;
};

#define sWaypointMgr WaypointMgr::instance()

#endif
