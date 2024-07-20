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
#ifndef MC_SIM_EDITOR_CGI_MANAGER_H_
#define MC_SIM_EDITOR_CGI_MANAGER_H_

#include <cgi/ManipulatorOrbit.h>
#include <cgi/ManipulatorTrack.h>

#include <cgi/CGI.h>
#include <cgi/HUD.h>
#include <cgi/Intersections.h>

namespace mc {

namespace pro {
    class Project;
} // namespace pro

namespace cgi
{

class Model;

/** */
class Manager
{
public:

    Manager();
    virtual ~Manager();

    void update();

    inline osgGA::CameraManipulator* getCameraManipulator()
    {
        return _manipulator.get();
    }

    inline std::shared_ptr<Data> getData() { return _data; }

    /** Returns root node. */
    inline osg::Group* getNodeCGI() { return _cgi->root().get(); }

    /** Returns root node. */
    inline osg::Group* getNodeHUD() { return _hud->root().get(); }

    void setCameraManipulatorOrbit();
    void setCameraManipulatorTrack();

    void setGridVisibility(bool grid_visible);

    void setProject(std::shared_ptr<pro::Project> proj);

    void setWinHeight(int h);
    void setWinWidth(int w);

private:

    std::shared_ptr<Data> _data;

    std::shared_ptr<CGI> _cgi;
    std::shared_ptr<HUD> _hud;

    std::shared_ptr<Intersections> _intersections;

    osg::ref_ptr<osgGA::CameraManipulator> _manipulator;    ///< current manipulator

    osg::ref_ptr<ManipulatorOrbit> _manipulatorOrbit;
    osg::ref_ptr<ManipulatorTrack> _manipulatorTrack;
};

} // namespace cgi
} // namespace mc

#endif // MC_SIM_EDITOR_CGI_MANAGER_H_
