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

#ifndef _AUTOBROADCASTMGR_H_
#define _AUTOBROADCASTMGR_H_

#include "Common.h"
#include <map>
#include <vector>

enum class AnnounceType : uint8
{
    World = 0,
    Notification = 1,
    Both = 2,
};

class AC_GAME_API AutobroadcastMgr
{
public:
    static AutobroadcastMgr* instance();

    void LoadAutobroadcasts();
    void LoadAutobroadcastsLocalized();
    void SendAutobroadcasts();

private:
    void SendWorldAnnouncement(uint8 textId);
    void SendNotificationAnnouncement(uint8 textId);

    using AutobroadcastsMap = std::map<uint8, std::vector<std::string>>;
    using AutobroadcastsWeightMap = std::map<uint8, uint8>;
    AutobroadcastsMap _autobroadcasts;                  // autobroadcast messages
    AutobroadcastsWeightMap _autobroadcastsWeights;    // Weights for each message

    AnnounceType _announceType;
};

#define sAutobroadcastMgr AutobroadcastMgr::instance()

#endif // _AUTOBROADCASTMGR_H_
