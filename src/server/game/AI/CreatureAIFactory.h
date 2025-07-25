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

#ifndef ACORE_CREATUREAIFACTORY_H
#define ACORE_CREATUREAIFACTORY_H

#include "FactoryHolder.h"
#include "ObjectRegistry.h"

using CreatureAICreator = FactoryHolder<CreatureAI, Creature>;
struct SelectableAI : public CreatureAICreator, public Permissible<Creature>
{
    SelectableAI(std::string const& name) : CreatureAICreator(name), Permissible<Creature>() { }
};

template<class REAL_AI>
struct CreatureAIFactory : public SelectableAI
{
    CreatureAIFactory(std::string const& name) : SelectableAI(name) { }

    inline CreatureAI* Create(Creature* c) const override
    {
        return new REAL_AI(c);
    }

    int32 Permit(Creature const* c) const override
    {
        return REAL_AI::Permissible(c);
    }
};

using CreatureAIRegistry = CreatureAICreator::FactoryHolderRegistry;
#define sCreatureAIRegistry CreatureAIRegistry::instance()

#endif
