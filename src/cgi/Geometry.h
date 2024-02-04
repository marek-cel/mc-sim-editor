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
#ifndef MC_SIM_EDITOR_CGI_GEOMETRY_H_
#define MC_SIM_EDITOR_CGI_GEOMETRY_H_

////////////////////////////////////////////////////////////////////////////////

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Group>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi { namespace Geometry
{

/**
 * @brief Creates line.
 * @param geom
 * @param b beginning coordinates
 * @param e end coordinates
 * @param color
 */
void CreateLine(osg::Geometry* geom, const osg::Vec3& b, const osg::Vec3& e,
                const osg::Vec4& color);

} // namespace Geometry
} // namespace cgi
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_CGI_GEOMETRY_H_
