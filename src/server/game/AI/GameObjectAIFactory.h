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

#ifndef ACORE_GAMEOBJECTAIFACTORY_H
#define ACORE_GAMEOBJECTAIFACTORY_H

#include "FactoryHolder.h"
#include "GameObjectAI.h"

using GameObjectAICreator = FactoryHolder<GameObjectAI, GameObject>;
struct SelectableGameObjectAI : public GameObjectAICreator, public Permissible<GameObject>
{
    SelectableGameObjectAI(std::string const& name) : GameObjectAICreator(name), Permissible<GameObject>() { }
};

template<class REAL_GO_AI>
struct GameObjectAIFactory : public SelectableGameObjectAI
{
    GameObjectAIFactory(std::string const& name) : SelectableGameObjectAI(name) { }

    GameObjectAI* Create(GameObject* go) const override
    {
        return new REAL_GO_AI(go);
    }

    int32 Permit(GameObject const* go) const override
    {
        return REAL_GO_AI::Permissible(go);
    }
};

using GameObjectAIRegistry = GameObjectAICreator::FactoryHolderRegistry;
#define sGameObjectAIRegistry GameObjectAIRegistry::instance()

#endif
