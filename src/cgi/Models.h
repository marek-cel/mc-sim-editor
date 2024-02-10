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
#ifndef MC_SIM_EDITOR_CGI_MODELS_H_
#define MC_SIM_EDITOR_CGI_MODELS_H_

#include <memory>

#include <osg/Node>

namespace mc {
namespace cgi {

/** 3D models. */
class Models
{
public:

    using List = std::map<std::string, osg::ref_ptr<osg::Node>>;

    static std::shared_ptr<Models> Instance();

    /** */
    static osg::Node* Get(std::string file);

    static void Reset();

private:

    static std::shared_ptr<Models> instance_;

    List list_;

    Models() = default;
    Models(const Models&) = delete;
    Models(Models&&) = delete;
};

} // namespace cgi
} // namespace mc

#endif // MC_SIM_EDITOR_CGI_MODELS_H_
