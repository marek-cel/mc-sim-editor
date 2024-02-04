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
#ifndef MC_SIM_EDITOR_CGI_GRID_H_
#define MC_SIM_EDITOR_CGI_GRID_H_

////////////////////////////////////////////////////////////////////////////////

#include <osg/Switch>

#include <cgi/Component.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

/** */
class Grid : public Component
{
public:

    static constexpr int size_ = 10;
    static constexpr int step_ = 1;

    /** */
    Grid(std::shared_ptr<Data> data);

    void Update() override;

private:

    osg::ref_ptr<osg::Switch> switch_;

    bool grid_visible_ = true;

    void CreateGrid();
    void CreateGridAuxLines();
    void CreateGridMainLines();
};

} // namespace cgi
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_CGI_GRID_H_
