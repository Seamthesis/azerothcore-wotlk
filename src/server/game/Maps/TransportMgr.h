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

#ifndef TRANSPORTMGR_H
#define TRANSPORTMGR_H

#include "DBCStores.h"
#include "ObjectGuid.h"
#include "Spline.h"
#include <G3D/Quat.h>

struct KeyFrame;
struct GameObjectTemplate;
struct TransportTemplate;
class Transport;
class StaticTransport;
class MotionTransport;
class Map;

using TransportSpline = Movement::Spline<double>;
using KeyFrameVec = std::vector<KeyFrame>;
using TransportTemplates = std::unordered_map<uint32, TransportTemplate>;
using TransportSet = std::set<MotionTransport*>;
using TransportMap = std::unordered_map<uint32, TransportSet>;
using TransportInstanceMap = std::unordered_map<uint32, std::set<uint32> >;
struct KeyFrame
{
    explicit KeyFrame(TaxiPathNodeEntry const* node) : Index(0), Node(node), InitialOrientation(0.0f),
        DistSinceStop(-1.0f), DistUntilStop(-1.0f), DistFromPrev(-1.0f), TimeFrom(0.0f), TimeTo(0.0f),
        Teleport(false), ArriveTime(0), DepartureTime(0), Spline(nullptr), NextDistFromPrev(0.0f), NextArriveTime(0)
    {
    }

    uint32 Index;
    TaxiPathNodeEntry const* Node;
    float InitialOrientation;
    float DistSinceStop;
    float DistUntilStop;
    float DistFromPrev;
    float TimeFrom;
    float TimeTo;
    bool Teleport;
    uint32 ArriveTime;
    uint32 DepartureTime;
    TransportSpline* Spline;

    // Data needed for next frame
    float NextDistFromPrev;
    uint32 NextArriveTime;

    bool IsTeleportFrame() const { return Teleport; }
    bool IsStopFrame() const { return Node->actionFlag == 2; }
};

struct TransportTemplate
{
    TransportTemplate() : inInstance(false), pathTime(0), accelTime(0.0f), accelDist(0.0f), entry(0) { }
    ~TransportTemplate();

    std::set<uint32> mapsUsed;
    bool inInstance;
    uint32 pathTime;
    KeyFrameVec keyFrames;
    float accelTime;
    float accelDist;
    uint32 entry;
};

using TransportPathContainer = std::map<uint32, TransportAnimationEntry const*>;
using TransportPathRotationContainer = std::map<uint32, TransportRotationEntry const*>;
struct TransportAnimation
{
    TransportAnimation() : TotalTime(0) { }

    TransportPathContainer Path;
    TransportPathRotationContainer Rotations;
    uint32 TotalTime;

    bool GetAnimNode(uint32 time, TransportAnimationEntry const*& curr, TransportAnimationEntry const*& next, float& percPos) const;
    void GetAnimRotation(uint32 time, G3D::Quat& curr, G3D::Quat& next, float& percRot) const;
};

using TransportAnimationContainer = std::map<uint32, TransportAnimation>;
class TransportMgr
{
    friend void LoadDBCStores(std::string const&);

public:
    static TransportMgr* instance();

    void Unload();

    void LoadTransportTemplates();

    // Creates a transport using given GameObject template entry
    MotionTransport* CreateTransport(uint32 entry, ObjectGuid::LowType guid = 0, Map* map = nullptr);

    // Spawns all continent transports, used at core startup
    void SpawnContinentTransports();

    // creates all transports for instance
    void CreateInstanceTransports(Map* map);

    TransportTemplate const* GetTransportTemplate(uint32 entry) const
    {
        TransportTemplates::const_iterator itr = _transportTemplates.find(entry);
        if (itr != _transportTemplates.end())
            return &itr->second;
        return nullptr;
    }

    TransportAnimation const* GetTransportAnimInfo(uint32 entry) const
    {
        TransportAnimationContainer::const_iterator itr = _transportAnimations.find(entry);
        if (itr != _transportAnimations.end())
            return &itr->second;

        return nullptr;
    }

    // Generates and precaches a path for transport to avoid generation each time transport instance is created
    void GeneratePath(GameObjectTemplate const* goInfo, TransportTemplate* transport);

    void AddPathNodeToTransport(uint32 transportEntry, uint32 timeSeg, TransportAnimationEntry const* node);

    void AddPathRotationToTransport(uint32 transportEntry, uint32 timeSeg, TransportRotationEntry const* node)
    {
        _transportAnimations[transportEntry].Rotations[timeSeg] = node;
    }

private:
    TransportMgr();
    ~TransportMgr();
    TransportMgr(TransportMgr const&);
    TransportMgr& operator=(TransportMgr const&);

    // Container storing transport templates
    TransportTemplates _transportTemplates;

    // Container storing transport entries to create for instanced maps
    TransportInstanceMap _instanceTransports;

    TransportAnimationContainer _transportAnimations;
};

#define sTransportMgr TransportMgr::instance()

#endif // TRANSPORTMGR_H
