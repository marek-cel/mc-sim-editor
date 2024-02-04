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
#ifndef MC_SIM_EDITOR_CGI_DATA_H_
#define MC_SIM_EDITOR_CGI_DATA_H_

////////////////////////////////////////////////////////////////////////////////

#include <cgi/Component.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

struct Data
{

    double camera_w = 1.0;
    double camera_x = 0.0;
    double camera_y = 0.0;
    double camera_z = 0.0;

    int win_width  = 0;
    int win_height = 0;

    bool grid_visible = false;
};

} // namespace cgi
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_CGI_DATA_H_
