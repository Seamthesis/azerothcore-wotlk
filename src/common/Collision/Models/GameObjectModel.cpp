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

#include "GameObjectModel.h"
#include "Log.h"
#include "MapTree.h"
#include "ModelInstance.h"
#include "Timer.h"
#include "VMapDefinitions.h"
#include "VMapFactory.h"
#include "VMapMgr2.h"
#include "WorldModel.h"

using G3D::Vector3;
using G3D::Ray;
using G3D::AABox;

struct GameobjectModelData
{
    GameobjectModelData(char const* name_, uint32 nameLength, Vector3 const& lowBound, Vector3 const& highBound, bool isWmo_) :
        bound(lowBound, highBound), name(name_, nameLength), isWmo(isWmo_) { }

    AABox bound;
    std::string name;
    bool isWmo;
};

using ModelList = std::unordered_map<uint32, GameobjectModelData>;
ModelList model_list;

void LoadGameObjectModelList(std::string const& dataPath)
{
    uint32 oldMSTime = getMSTime();

    FILE* model_list_file = fopen((dataPath + "vmaps/" + VMAP::GAMEOBJECT_MODELS).c_str(), "rb");
    if (!model_list_file)
    {
        LOG_ERROR("maps", "Unable to open '{}' file.", VMAP::GAMEOBJECT_MODELS);
        return;
    }

    char magic[8];
    if (fread(magic, 1, 8, model_list_file) != 8 || memcmp(magic, VMAP::VMAP_MAGIC, 8) != 0)
    {
        LOG_ERROR("maps", "File '{}' has wrong header, expected {}.", VMAP::GAMEOBJECT_MODELS, VMAP::VMAP_MAGIC);
        fclose(model_list_file);
        return;
    }

    uint32 name_length, displayId;
    uint8 isWmo;
    char buff[500];
    while (true)
    {
        Vector3 v1, v2;
        if (fread(&displayId, sizeof(uint32), 1, model_list_file) != 1)
            if (feof(model_list_file))  // EOF flag is only set after failed reading attempt
            {
                break;
            }

        if (fread(&isWmo, sizeof(uint8), 1, model_list_file) != 1
                || fread(&name_length, sizeof(uint32), 1, model_list_file) != 1
                || name_length >= sizeof(buff)
                || fread(&buff, sizeof(char), name_length, model_list_file) != name_length
                || fread(&v1, sizeof(Vector3), 1, model_list_file) != 1
                || fread(&v2, sizeof(Vector3), 1, model_list_file) != 1)
        {
            LOG_ERROR("maps", "File '{}' seems to be corrupted!", VMAP::GAMEOBJECT_MODELS);
            fclose(model_list_file);
            break;
        }

        if (v1.isNaN() || v2.isNaN())
        {
            LOG_ERROR("maps", "File '{}' Model '{}' has invalid v1{} v2{} values!",
                      VMAP::GAMEOBJECT_MODELS, std::string(buff, name_length), v1.toString(), v2.toString());
            continue;
        }

        model_list.emplace(std::piecewise_construct, std::forward_as_tuple(displayId), std::forward_as_tuple(&buff[0], name_length, v1, v2, isWmo != 0));
    }

    fclose(model_list_file);

    LOG_INFO("server.loading", ">> Loaded {} GameObject Models in {} ms", uint32(model_list.size()), GetMSTimeDiffToNow(oldMSTime));
    LOG_INFO("server.loading", " ");
}

GameObjectModel::~GameObjectModel()
{
    if (iModel)
    {
        VMAP::VMapFactory::createOrGetVMapMgr()->releaseModelInstance(name);
    }
}

bool GameObjectModel::initialize(std::unique_ptr<GameObjectModelOwnerBase> modelOwner, std::string const& dataPath)
{
    ModelList::const_iterator it = model_list.find(modelOwner->GetDisplayId());
    if (it == model_list.end())
    {
        return false;
    }

    G3D::AABox mdl_box(it->second.bound);
    // ignore models with no bounds
    if (mdl_box == G3D::AABox::zero())
    {
        LOG_ERROR("maps", "GameObject model {} has zero bounds, loading skipped", it->second.name);
        return false;
    }

    iModel = VMAP::VMapFactory::createOrGetVMapMgr()->acquireModelInstance(dataPath + "vmaps/", it->second.name,
        it->second.isWmo ? VMAP::ModelFlags::MOD_WORLDSPAWN : VMAP::ModelFlags::MOD_M2);

    if (!iModel)
    {
        return false;
    }

    name = it->second.name;
    iPos = modelOwner->GetPosition();
    phasemask = modelOwner->GetPhaseMask();
    iScale = modelOwner->GetScale();
    iInvScale = 1.f / iScale;

    G3D::Matrix3 iRotation = G3D::Matrix3::fromEulerAnglesZYX(modelOwner->GetOrientation(), 0, 0);
    iInvRot = iRotation.inverse();
    // transform bounding box:
    mdl_box = AABox(mdl_box.low() * iScale, mdl_box.high() * iScale);
    AABox rotated_bounds;
    for (int i = 0; i < 8; ++i)
    {
        rotated_bounds.merge(iRotation * mdl_box.corner(i));
    }

    iBound = rotated_bounds + iPos;

#ifdef SPAWN_CORNERS
    // test:
    for (int i = 0; i < 8; ++i)
    {
        Vector3 pos(iBound.corner(i));
        modelOwner->DebugVisualizeCorner(pos);
    }
#endif

    owner = std::move(modelOwner);
    isWmo = it->second.isWmo;
    return true;
}

GameObjectModel* GameObjectModel::Create(std::unique_ptr<GameObjectModelOwnerBase> modelOwner, std::string const& dataPath)
{
    GameObjectModel* mdl = new GameObjectModel();
    if (!mdl->initialize(std::move(modelOwner), dataPath))
    {
        delete mdl;
        return nullptr;
    }

    return mdl;
}

bool GameObjectModel::intersectRay(const G3D::Ray& ray, float& MaxDist, bool StopAtFirstHit, uint32 ph_mask, VMAP::ModelIgnoreFlags ignoreFlags) const
{
    if (!(phasemask & ph_mask) || !owner->IsSpawned())
    {
        return false;
    }

    float time = ray.intersectionTime(iBound);
    if (time == G3D::inf())
    {
        return false;
    }

    // child bounds are defined in object space:
    Vector3 p = iInvRot * (ray.origin() - iPos) * iInvScale;
    Ray modRay(p, iInvRot * ray.direction());
    float distance = MaxDist * iInvScale;
    bool hit = iModel->IntersectRay(modRay, distance, StopAtFirstHit, ignoreFlags);
    if (hit)
    {
        distance *= iScale;
        MaxDist = distance;
    }
    return hit;
}

void GameObjectModel::IntersectPoint(G3D::Vector3 const& point, VMAP::AreaInfo& info, uint32 ph_mask) const
{
    if (!(phasemask & ph_mask) || !owner->IsSpawned() || !IsMapObject())
        return;

    if (!iBound.contains(point))
        return;

    // child bounds are defined in object space:
    Vector3 pModel = iInvRot * (point - iPos) * iInvScale;
    Vector3 zDirModel = iInvRot * Vector3(0.f, 0.f, -1.f);
    float zDist;
    if (iModel->IntersectPoint(pModel, zDirModel, zDist, info))
    {
        Vector3 modelGround = pModel + zDist * zDirModel;
        float world_Z = ((modelGround * iInvRot) * iScale + iPos).z;
        if (info.ground_Z < world_Z)
            info.ground_Z = world_Z;
    }
}

bool GameObjectModel::GetLocationInfo(G3D::Vector3 const& point, VMAP::LocationInfo& info, uint32 ph_mask) const
{
    if (!(phasemask & ph_mask) || !owner->IsSpawned() || !IsMapObject())
        return false;

    if (!iBound.contains(point))
        return false;

    // child bounds are defined in object space:
    Vector3 pModel = iInvRot * (point - iPos) * iInvScale;
    Vector3 zDirModel = iInvRot * Vector3(0.f, 0.f, -1.f);
    float zDist;
    if (iModel->GetLocationInfo(pModel, zDirModel, zDist, info))
    {
        Vector3 modelGround = pModel + zDist * zDirModel;
        float world_Z = ((modelGround * iInvRot) * iScale + iPos).z;
        if (info.ground_Z < world_Z)
        {
            info.ground_Z = world_Z;
            return true;
        }
    }

    return false;
}

bool GameObjectModel::GetLiquidLevel(G3D::Vector3 const& point, VMAP::LocationInfo& info, float& liqHeight) const
{
    // child bounds are defined in object space:
    Vector3 pModel = iInvRot * (point - iPos) * iInvScale;
    //Vector3 zDirModel = iInvRot * Vector3(0.f, 0.f, -1.f);
    float zDist;
    if (info.hitModel->GetLiquidLevel(pModel, zDist))
    {
        // calculate world height (zDist in model coords):
        // assume WMO not tilted (wouldn't make much sense anyway)
        liqHeight = zDist * iScale + iPos.z;
        return true;
    }
    return false;
}

bool GameObjectModel::UpdatePosition()
{
    if (!iModel)
    {
        return false;
    }

    ModelList::const_iterator it = model_list.find(owner->GetDisplayId());
    if (it == model_list.end())
    {
        return false;
    }

    G3D::AABox mdl_box(it->second.bound);

    // ignore models with no bounds
    if (mdl_box == G3D::AABox::zero())
    {
        //VMAP_ERROR_LOG("misc", "GameObject model %s has zero bounds, loading skipped", it->second.name.c_str());
        return false;
    }

    iPos = owner->GetPosition();
    G3D::Matrix3 iRotation = G3D::Matrix3::fromEulerAnglesZYX(owner->GetOrientation(), 0, 0);
    iInvRot = iRotation.inverse();

    // transform bounding box:
    mdl_box = AABox(mdl_box.low() * iScale, mdl_box.high() * iScale);
    AABox rotated_bounds;

    for (int i = 0; i < 8; ++i)
    {
        rotated_bounds.merge(iRotation * mdl_box.corner(i));
    }

    iBound = rotated_bounds + iPos;
#ifdef SPAWN_CORNERS
    // test:
    for (int i = 0; i < 8; ++i)
    {
        Vector3 pos(iBound.corner(i));
        owner->DebugVisualizeCorner(pos);
    }
#endif

    return true;
}
