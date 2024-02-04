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

#include <cgi/Builder.h>

#include <cgi/Grid.h>
#include <cgi/Model.h>
#include <cgi/ViewGizmo.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

////////////////////////////////////////////////////////////////////////////////

void Builder::BuildCGI(std::shared_ptr<Data> data, std::shared_ptr<Component> root)
{
    // grid
    std::shared_ptr<Grid> grid = std::make_shared<Grid>(data);
    root->AddChild(grid);

    // model
    std::shared_ptr<Model> mod = std::make_shared<Model>(data);
    root->AddChild(mod);
}

////////////////////////////////////////////////////////////////////////////////

void Builder::BuildHUD(std::shared_ptr<Data> data, std::shared_ptr<Component> root)
{
    // gizmo
    std::shared_ptr<ViewGizmo> gizmo = std::make_shared<ViewGizmo>(data);
    root->AddChild(gizmo);
}

////////////////////////////////////////////////////////////////////////////////

} // namespace cgi
} // namespace mc
