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

namespace mc {
namespace cgi {

Manager::Manager()
{
    _data = std::make_shared<Data>();

    _cgi = std::make_shared<CGI>(_data);
    _hud = std::make_shared<HUD>(_data);

    Builder::buildCGI(_data, _cgi);
    Builder::buildHUD(_data, _hud);

    _manipulatorOrbit = new ManipulatorOrbit();
    _manipulatorTrack = new ManipulatorTrack();

    _manipulator = _manipulatorTrack;
}

Manager::~Manager() {}

void Manager::update()
{
    osg::Quat q = _manipulator->getMatrix().getRotate();
    osg::Vec3 v = _manipulator->getMatrix().getTrans();

    _data->camera_w = q.w();
    _data->camera_x = q.x();
    _data->camera_y = q.y();
    _data->camera_z = q.z();

    _data->camera_dist = v.length();

    _cgi->update();
    _hud->update();
}

void Manager::setCameraManipulatorOrbit()
{
    _manipulator = _manipulatorOrbit;
    _manipulatorOrbit->setTrackNode(_cgi->root().get());
}

void Manager::setCameraManipulatorTrack()
{
    _manipulator = _manipulatorTrack;
    _manipulatorTrack->setNode(_cgi->root().get());
}

void Manager::setGridVisibility(bool grid_visible)
{
    _data->grid_visible = grid_visible;
}

void Manager::setProject(std::shared_ptr<pro::Project> proj)
{
    _cgi->setProject(proj);
    _hud->setProject(proj);
}

void Manager::setWinHeight(int h)
{
    _data->win_height = h;
}

void Manager::setWinWidth(int w)
{
    _data->win_width = w;
}

} // namespace cgi
} // namespace mc
