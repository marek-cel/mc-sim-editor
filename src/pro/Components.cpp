/****************************************************************************//*
 *  Copyright (C) 2024 Marek M. Cel
 *
 *  This file is part of MC-Sim Editor.
 *
 *  MC-Sim Editor is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MC-Mass is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 ******************************************************************************/

#include <pro/Components.h>

#include <pro/Box.h>
#include <pro/File.h>
#include <pro/FLOLS.h>
#include <pro/Group.h>
#include <pro/LOD.h>
#include <pro/PAT.h>
#include <pro/Rotor.h>
#include <pro/Switch.h>
#include <pro/Trans.h>

#ifdef MCSIM_EDITOR_EXTRA_COMPONENTS
#endif // MCSIM_EDITOR_EXTRA_COMPONENTS

namespace mc {
namespace pro {

std::shared_ptr<Components> Components::_instance;

std::shared_ptr<Components> Components::instance()
{
    if ( !_instance )
    {
        Components* instance = new Components();
        _instance = std::shared_ptr<Components>(instance);
        _instance->addTypes();
    }

    return _instance;
}

void Components::addType(QString name, std::shared_ptr<Component> comp,
                         bool extra)
{
    Type type;

    type.name = name;
    type.component = comp;
    type.can_be_animated = comp->canBeAnimated();
    type.can_be_parent = comp->canBeParent();
    type.extra = extra;

    _types.push_back(type);
}

void Components::addTypes()
{
    addType("Box"              , std::make_shared< pro::Box    >());
    addType("File"             , std::make_shared< pro::File   >());
    addType("FLOLS"            , std::make_shared< pro::FLOLS  >());
    addType("Group"            , std::make_shared< pro::Group  >());
    addType("LOD"              , std::make_shared< pro::LOD    >());
    addType("PAT"              , std::make_shared< pro::PAT    >());
    addType("Rotor"            , std::make_shared< pro::Rotor  >());
    addType("Switch"           , std::make_shared< pro::Switch >());
    addType("Matrix Transform" , std::make_shared< pro::Trans  >());
#   ifdef MCSIM_EDITOR_EXTRA_COMPONENTS
#   endif // MCSIM_EDITOR_EXTRA_COMPONENTS
}

Components::Type Components::getComponentByTagName(QString tag_name)
{
    for ( auto type : _types )
    {
        if ( QString(type.component->getTagName()) == tag_name )
        {
            return type;
        }
    }

    return Type();
}

int Components::getIndexByTagName(QString tag_name)
{
    for ( size_t i = 0; i < _types.size(); ++i )
    {
        if ( QString(_types.at(i).component->getTagName()) == tag_name )
        {
            return i;
        }
    }

    return -1;
}

} // namespace pro
} // namespace mc
