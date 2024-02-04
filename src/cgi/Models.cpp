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

#include <cgi/Models.h>

#include <osgDB/ReadFile>

#include <mcutils/misc/MapUtils.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<Models> Models::instance_;

////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<Models> Models::Instance()
{
    if ( !instance_ )
    {
        Models* instance = new Models();
        instance_ = std::shared_ptr<Models>(instance);
    }

    return instance_;
}

////////////////////////////////////////////////////////////////////////////////

osg::Node* Models::Get(std::string file)
{
    osg::ref_ptr<osg::Node> model = GetMapItemByKey(&Instance()->list_, file);

    if ( model.valid() )
    {
        return model.get();
    }

    model = osgDB::readNodeFile(file);

    if ( model.valid() )
    {
        AddMapItem(&Instance()->list_, file, model);
        return model.get();
    }
    else
    {
        std::cerr << "Error! Cannot open object file: " << file << std::endl;
    }

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void Models::Reset()
{
    Instance()->list_.clear();
}

////////////////////////////////////////////////////////////////////////////////

} // namespace cgi
} // namespace mc
