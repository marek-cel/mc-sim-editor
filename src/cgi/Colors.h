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
#ifndef MC_SIM_EDITOR_CGI_COLORS_H_
#define MC_SIM_EDITOR_CGI_COLORS_H_

#include <osg/Vec3>

namespace mc {
namespace cgi {
namespace Colors {

const osg::Vec3 black   = osg::Vec3(0.00f, 0.00f, 0.00f);
const osg::Vec3 white   = osg::Vec3(1.00f, 1.00f, 1.00f);
const osg::Vec3 blue    = osg::Vec3(0.00f, 0.00f, 1.00f);
const osg::Vec3 cyan    = osg::Vec3(0.00f, 1.00f, 1.00f);
const osg::Vec3 green   = osg::Vec3(0.00f, 0.50f, 0.00f);
const osg::Vec3 grey    = osg::Vec3(0.50f, 0.50f, 0.50f);
const osg::Vec3 lime    = osg::Vec3(0.00f, 1.00f, 0.00f);
const osg::Vec3 magenta = osg::Vec3(1.00f, 0.00f, 1.00f);
const osg::Vec3 maroon  = osg::Vec3(0.50f, 0.00f, 0.00f);
const osg::Vec3 navy    = osg::Vec3(0.00f, 0.00f, 0.50f);
const osg::Vec3 olive   = osg::Vec3(0.50f, 0.50f, 0.00f);
const osg::Vec3 orange  = osg::Vec3(1.00f, 0.65f, 0.00f);
const osg::Vec3 purple  = osg::Vec3(0.50f, 0.00f, 0.50f);
const osg::Vec3 red     = osg::Vec3(1.00f, 0.00f, 0.00f);
const osg::Vec3 teal    = osg::Vec3(0.00f, 0.50f, 0.50f);
const osg::Vec3 yellow  = osg::Vec3(1.00f, 1.00f, 0.00f);
const osg::Vec3 amber   = osg::Vec3(1.00f, 0.14f, 0.00f);
const osg::Vec3 scarlet = osg::Vec3(1.00f, 0.75f, 0.00f);

const osg::Vec3 orangeRed = osg::Vec3(1.00f, 0.27f, 0.00f);

} // namespace Colors
} // namespace cgi
} // namespace mc

#endif // MC_SIM_EDITOR_CGI_COLORS_H_
