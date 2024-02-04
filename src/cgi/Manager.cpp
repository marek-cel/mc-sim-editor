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

#include <cgi/Manager.h>

#include <cgi/Builder.h>
#include <cgi/Model.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace cgi
{

////////////////////////////////////////////////////////////////////////////////

Manager::Manager()
{
    data_ = std::make_shared<Data>();

    cgi_ = std::make_shared<CGI>(data_);
    hud_ = std::make_shared<HUD>(data_);

    Builder::BuildCGI(data_, cgi_);
    Builder::BuildHUD(data_, hud_);

    _manipulatorOrbit = new ManipulatorOrbit();
    _manipulatorTrack = new ManipulatorTrack();

    _manipulator = _manipulatorTrack;
}

////////////////////////////////////////////////////////////////////////////////

Manager::~Manager() {}

////////////////////////////////////////////////////////////////////////////////

void Manager::Update()
{
    osg::Quat q = _manipulator->getMatrix().getRotate();

    data_->camera_w = q.w();
    data_->camera_x = q.x();
    data_->camera_y = q.y();
    data_->camera_z = q.z();

    cgi_->Update();
    hud_->Update();
}

////////////////////////////////////////////////////////////////////////////////

void Manager::SetCameraManipulatorOrbit()
{
    _manipulator = _manipulatorOrbit;
    _manipulatorOrbit->setTrackNode(cgi_->root().get());
}

////////////////////////////////////////////////////////////////////////////////

void Manager::SetCameraManipulatorTrack()
{
    _manipulator = _manipulatorTrack;
    _manipulatorTrack->setNode(cgi_->root().get());
}

////////////////////////////////////////////////////////////////////////////////

void Manager::SetGridVisibility(bool grid_visible)
{
    data_->grid_visible = grid_visible;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::SetProject(std::shared_ptr<pro::Project> proj)
{
    cgi_->SetProject(proj);
    hud_->SetProject(proj);
}

////////////////////////////////////////////////////////////////////////////////

void Manager::SetWinHeight(int h)
{
    data_->win_height = h;
}

////////////////////////////////////////////////////////////////////////////////

void Manager::SetWinWidth(int w)
{
    data_->win_width = w;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace cgi
} // namespace mc
