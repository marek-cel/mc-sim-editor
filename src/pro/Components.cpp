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

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace pro
{

////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<Components> Components::instance_;

////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<Components> Components::Instance()
{
    if ( !instance_ )
    {
        Components* instance = new Components();
        instance_ = std::shared_ptr<Components>(instance);
        instance_->AddTypes();
    }

    return instance_;
}

////////////////////////////////////////////////////////////////////////////////

void Components::AddType(QString name, std::shared_ptr<Component> comp,
                         bool extra)
{
    Type type;

    type.name = name;
    type.component = comp;
    type.can_be_animated = comp->CanBeAnimated();
    type.can_be_parent = comp->CanBeParent();
    type.extra = extra;

    types_.push_back(type);
}

////////////////////////////////////////////////////////////////////////////////

void Components::AddTypes()
{
    AddType("Box"              , std::make_shared< pro::Box    >());
    AddType("File"             , std::make_shared< pro::File   >());
    AddType("FLOLS"            , std::make_shared< pro::FLOLS  >());
    AddType("Group"            , std::make_shared< pro::Group  >());
    AddType("LOD"              , std::make_shared< pro::LOD    >());
    AddType("PAT"              , std::make_shared< pro::PAT    >());
    AddType("Rotor"            , std::make_shared< pro::Rotor  >());
    AddType("Switch"           , std::make_shared< pro::Switch >());
    AddType("Matrix Transform" , std::make_shared< pro::Trans  >());
#   ifdef MCSIM_EDITOR_EXTRA_COMPONENTS
#   endif // MCSIM_EDITOR_EXTRA_COMPONENTS
}

////////////////////////////////////////////////////////////////////////////////

Components::Type Components::GetComponentByTagName(QString tag_name)
{
    for ( auto type : types_ )
    {
        if ( QString(type.component->GetTagName()) == tag_name )
        {
            return type;
        }
    }

    return Type();
}

////////////////////////////////////////////////////////////////////////////////

int Components::GetIndexByTagName(QString tag_name)
{
    for ( size_t i = 0; i < types_.size(); ++i )
    {
        if ( QString(types_.at(i).component->GetTagName()) == tag_name )
        {
            return i;
        }
    }

    return -1;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace pro
} // namespace mc
