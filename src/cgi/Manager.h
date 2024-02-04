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

////////////////////////////////////////////////////////////////////////////////

#include <cgi/ManipulatorOrbit.h>
#include <cgi/ManipulatorTrack.h>

#include <cgi/CGI.h>
#include <cgi/HUD.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc
{

namespace pro
{
    class Project;
} // namespace pro

namespace cgi
{

class Model;

/** */
class Manager
{
public:

    /** */
    Manager();

    /** */
    virtual ~Manager();

    /** */
    void Update();

    /** */
    inline osgGA::CameraManipulator* GetCameraManipulator()
    {
        return _manipulator.get();
    }

    /** Returns root node. */
    inline osg::Group* GetNodeCGI() { return cgi_->root().get(); }

    /** Returns root node. */
    inline osg::Group* GetNodeHUD() { return hud_->root().get(); }

    void SetCameraManipulatorOrbit();
    void SetCameraManipulatorTrack();

    void SetGridVisibility(bool grid_visible);

    void SetProject(std::shared_ptr<pro::Project> proj);

    void SetWinHeight(int h);
    void SetWinWidth(int w);

private:

    std::shared_ptr<Data> data_;

    std::shared_ptr<CGI> cgi_;
    std::shared_ptr<HUD> hud_;

    osg::ref_ptr<osgGA::CameraManipulator> _manipulator;    ///< current manipulator

    osg::ref_ptr<ManipulatorOrbit> _manipulatorOrbit;
    osg::ref_ptr<ManipulatorTrack> _manipulatorTrack;
};

} // namespace cgi
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_CGI_MANAGER_H_
