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
#ifndef MC_SIM_EDITOR_CGI_VIEWGIZMO_H_
#define MC_SIM_EDITOR_CGI_VIEWGIZMO_H_

////////////////////////////////////////////////////////////////////////////////

#include <osg/PositionAttitudeTransform>

#include <cgi/Component.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

class ViewGizmo : public Component
{
public:

    static constexpr int offset_x_ = 15;
    static constexpr int offset_y_ = 15;

    /**
     * @brief Constructor.
     * @param parent parent module
     */
    ViewGizmo(std::shared_ptr<Data> data);

    void Update() override;

private:

    osg::ref_ptr<osg::PositionAttitudeTransform> pat_;

    int x0_ = 0;
    int y0_ = 0;

    void CreateGizmo();

};

} // namespace cgi
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_CGI_VIEWGIZMO_H_
