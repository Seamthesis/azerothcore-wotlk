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

#ifndef _VMAPMANAGER2_H
#define _VMAPMANAGER2_H

#include "IVMapMgr.h"
#include <mutex>
#include <unordered_map>
#include <vector>

//===========================================================

#define MAP_FILENAME_EXTENSION2 ".vmtree"

#define FILENAMEBUFFER_SIZE 500

/**
This is the main Class to manage loading and unloading of maps, line of sight, height calculation and so on.
For each map or map tile to load it reads a directory file that contains the ModelContainer files used by this map or map tile.
Each global map or instance has its own dynamic BSP-Tree.
The loaded ModelContainers are included in one of these BSP-Trees.
Additionally a table to match map ids and map names is used.
*/

//===========================================================

namespace G3D
{
    class Vector3;
}

namespace VMAP
{
    class StaticMapTree;
    class WorldModel;

    class ManagedModel
    {
    public:
        ManagedModel()  { }
        void setModel(WorldModel* model) { iModel = model; }
        WorldModel* getModel() { return iModel; }
        int decRefCount() { return --iRefCount; }
    protected:
        WorldModel* iModel{nullptr};
        int iRefCount{0};
    };

    using InstanceTreeMap = std::unordered_map<uint32, StaticMapTree*>;
    using ModelFileMap = std::unordered_map<std::string, ManagedModel>;
    enum DisableTypes
    {
        VMAP_DISABLE_AREAFLAG       = 0x1,
        VMAP_DISABLE_HEIGHT         = 0x2,
        VMAP_DISABLE_LOS            = 0x4,
        VMAP_DISABLE_LIQUIDSTATUS   = 0x8
    };

    class VMapMgr2 : public IVMapMgr
    {
    protected:
        // Tree to check collision
        ModelFileMap iLoadedModelFiles;
        InstanceTreeMap iInstanceMapTrees;
        bool thread_safe_environment;

        // Mutex for iLoadedModelFiles
        std::mutex LoadedModelFilesLock;

        bool _loadMap(uint32 mapId, const std::string& basePath, uint32 tileX, uint32 tileY);
        /* void _unloadMap(uint32 pMapId, uint32 x, uint32 y); */

        static uint32 GetLiquidFlagsDummy(uint32) { return 0; }
        static bool IsVMAPDisabledForDummy(uint32 /*entry*/, uint8 /*flags*/) { return false; }

        InstanceTreeMap::const_iterator GetMapTree(uint32 mapId) const;

    public:
        // public for debug
        [[nodiscard]] G3D::Vector3 convertPositionToInternalRep(float x, float y, float z) const;
        static std::string getMapFileName(unsigned int mapId);

        VMapMgr2();
        ~VMapMgr2() override;

        void InitializeThreadUnsafe(const std::vector<uint32>& mapIds);

        int loadMap(const char* pBasePath, unsigned int mapId, int x, int y) override;

        void unloadMap(unsigned int mapId, int x, int y) override;
        void unloadMap(unsigned int mapId) override;

        bool isInLineOfSight(unsigned int mapId, float x1, float y1, float z1, float x2, float y2, float z2, ModelIgnoreFlags ignoreFlags) override ;
        /**
        fill the hit pos and return true, if an object was hit
        */
        bool GetObjectHitPos(unsigned int mapId, float x1, float y1, float z1, float x2, float y2, float z2, float& rx, float& ry, float& rz, float modifyDist) override;
        float getHeight(unsigned int mapId, float x, float y, float z, float maxSearchDist) override;

        bool processCommand(char* /*command*/) override { return false; } // for debug and extensions

        bool GetAreaInfo(uint32 pMapId, float x, float y, float& z, uint32& flags, int32& adtId, int32& rootId, int32& groupId) const override;
        bool GetLiquidLevel(uint32 pMapId, float x, float y, float z, uint8 reqLiquidType, float& level, float& floor, uint32& type, uint32& mogpFlags) const override;
        void GetAreaAndLiquidData(uint32 mapId, float x, float y, float z, uint8 reqLiquidType, AreaAndLiquidData& data) const override;

        WorldModel* acquireModelInstance(const std::string& basepath, const std::string& filename, uint32 flags);
        void releaseModelInstance(const std::string& filename);

        // what's the use of this? o.O
        [[nodiscard]] std::string getDirFileName(unsigned int mapId, int /*x*/, int /*y*/) const override
        {
            return getMapFileName(mapId);
        }
        LoadResult existsMap(const char* basePath, unsigned int mapId, int x, int y) override;
        void GetInstanceMapTree(InstanceTreeMap& instanceMapTree);

        typedef uint32(*GetLiquidFlagsFn)(uint32 liquidType);
        GetLiquidFlagsFn GetLiquidFlagsPtr;

        typedef bool(*IsVMAPDisabledForFn)(uint32 entry, uint8 flags);
        IsVMAPDisabledForFn IsVMAPDisabledForPtr;
    };
}

#endif
