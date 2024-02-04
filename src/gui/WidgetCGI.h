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
#ifndef MC_SIM_EDITOR_GUI_WIDGETCGI_H_
#define MC_SIM_EDITOR_GUI_WIDGETCGI_H_

////////////////////////////////////////////////////////////////////////////////

#include <QShortcut>

#include <cgi/Manager.h>
#include <gui/WidgetOSG.h>
#include <pro/Project.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui
{

/**
 * @brief CGI widget class.
 */
class WidgetCGI : public WidgetOSG
{
    Q_OBJECT

public:

    /** @brief Constructor. */
    explicit WidgetCGI(QWidget* parent = nullptr);

    /** @brief Destructor. */
    virtual ~WidgetCGI();

    cgi::Manager* getManagerCGI() { return manager_cgi_; }

public slots:

    void resetView();
    void topView();
    void bottomView();
    void frontView();
    void backView();
    void leftView();
    void rightView();

    void setProject(std::shared_ptr<pro::Project> proj);

    void setAnimationTime(double time);

    void setCameraManipulatorOrbit();
    void setCameraManipulatorTrack();

protected:

    /** */
    void resizeEvent(QResizeEvent* event) override;

    /** */
    void timerEvent(QTimerEvent* event) override;

private:

    QShortcut* shortcut_view_default_ = nullptr;    ///<
    QShortcut* shortcut_view_top_     = nullptr;    ///<
    QShortcut* shortcut_view_back_    = nullptr;    ///<
    QShortcut* shortcut_view_left_    = nullptr;    ///<
    QShortcut* shortcut_view_bottom_  = nullptr;    ///<
    QShortcut* shortcut_view_front_   = nullptr;    ///<
    QShortcut* shortcut_view_right_   = nullptr;    ///<

    cgi::Manager* manager_cgi_ = nullptr;           ///<

    osg::ref_ptr<osg::Camera> camera_cgi_;          ///<
    osg::ref_ptr<osg::Camera> camera_hud_;          ///<

    std::weak_ptr<pro::Project> proj_;

    double anim_time_ = 0.0;

    int timer_id_ = 0;                              ///< timer ID

    /** */
    QWidget* addViewWidget();

    void createCameras();
};

} // namespace gui
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_GUI_WIDGETCGI_H_
