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

////////////////////////////////////////////////////////////////////////////////

#include <osg/Vec3>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

/**
 * @brief Colors class.
 */
struct Colors
{
    static const osg::Vec3 black;       ///< black
    static const osg::Vec3 white;       ///< white

    static const osg::Vec3 blue;        ///< cyan    (according to W3C)
    static const osg::Vec3 cyan;        ///< cyan    (according to W3C)
    static const osg::Vec3 green;       ///< green   (according to W3C)
    static const osg::Vec3 grey;        ///< grey    (according to W3C)
    static const osg::Vec3 lime;        ///< lime    (according to W3C)
    static const osg::Vec3 magenta;     ///< magenta (according to W3C)
    static const osg::Vec3 maroon;      ///< maroon  (according to W3C)
    static const osg::Vec3 navy;        ///< navy    (according to W3C)
    static const osg::Vec3 olive;       ///< olive   (according to W3C)
    static const osg::Vec3 orange;      ///< orange  (according to W3C)
    static const osg::Vec3 purple;      ///< purple  (according to W3C)
    static const osg::Vec3 red;         ///< red     (according to W3C)
    static const osg::Vec3 teal;        ///< teal    (according to W3C)
    static const osg::Vec3 yellow;      ///< yellow  (according to W3C)

    static const osg::Vec3 amber;       ///< amber
    static const osg::Vec3 scarlet;     ///< scarlet

    static const osg::Vec3 orangeRed;   ///< OrangeRed (according to W3C)
};

} // namespace cgi
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_CGI_COLORS_H_
