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

#include <gui/WidgetCGI.h>

#include <osgGA/StateSetManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <cgi/Model.h>

namespace mc {
namespace gui {

WidgetCGI::WidgetCGI(QWidget* parent)
    : WidgetOSG(parent)
{
    _manager_cgi = new cgi::Manager();

    QWidget *widget = addViewWidget();

    _layout = new QGridLayout(this);
    _layout->setContentsMargins(1, 1, 1, 1);
    _layout->addWidget(widget, 0, 0);

    setLayout(_layout);

    _shortcut_view_default = new QShortcut( QKeySequence(Qt::Key_0), this, SLOT(resetView()) );
    _shortcut_view_top     = new QShortcut( QKeySequence(Qt::Key_7), this, SLOT(topView())   );
    _shortcut_view_back    = new QShortcut( QKeySequence(Qt::Key_3), this, SLOT(backView())   );
    _shortcut_view_left    = new QShortcut( QKeySequence(Qt::Key_1), this, SLOT(leftView())   );
    _shortcut_view_bottom  = new QShortcut( QKeySequence(Qt::CTRL + Qt::Key_7), this, SLOT(bottomView()) );
    _shortcut_view_front   = new QShortcut( QKeySequence(Qt::CTRL + Qt::Key_3), this, SLOT(frontView()) );
    _shortcut_view_right   = new QShortcut( QKeySequence(Qt::CTRL + Qt::Key_1), this, SLOT(rightView()) );

    setCameraManipulatorTrack();
    resetView();

    _timerId = startTimer(1000.0 / 60.0);
}

WidgetCGI::~WidgetCGI()
{
    if ( _timerId ) killTimer(_timerId);

    if ( _manager_cgi ) { delete _manager_cgi; } _manager_cgi = nullptr;
}

void WidgetCGI::resetView()
{
    osg::ref_ptr<osgGA::OrbitManipulator> manipulator =
            dynamic_cast<osgGA::OrbitManipulator*>(_manager_cgi->getCameraManipulator());

    manipulator->setHeading(-M_PI_4);
    manipulator->setElevation(M_PI_2 / 3.0);
    manipulator->setCenter(osg::Vec3d(0.0,0.0,0.0));
}

void WidgetCGI::topView()
{
    osg::ref_ptr<osgGA::OrbitManipulator> manipulator =
            dynamic_cast<osgGA::OrbitManipulator*>(_manager_cgi->getCameraManipulator());

    manipulator->setHeading(0.0);
    manipulator->setElevation(M_PI_2);
}

void WidgetCGI::bottomView()
{
    osg::ref_ptr<osgGA::OrbitManipulator> manipulator =
            dynamic_cast<osgGA::OrbitManipulator*>(_manager_cgi->getCameraManipulator());

    manipulator->setHeading(0.0);
    manipulator->setElevation(-M_PI_2);
}

void WidgetCGI::frontView()
{
    osg::ref_ptr<osgGA::OrbitManipulator> manipulator =
            dynamic_cast<osgGA::OrbitManipulator*>(_manager_cgi->getCameraManipulator());

    manipulator->setHeading(-M_PI_2);
    manipulator->setElevation(0.0);
}

void WidgetCGI::backView()
{
    osg::ref_ptr<osgGA::OrbitManipulator> manipulator =
            dynamic_cast<osgGA::OrbitManipulator*>(_manager_cgi->getCameraManipulator());

    manipulator->setHeading(M_PI_2);
    manipulator->setElevation(0.0);
}

void WidgetCGI::leftView()
{
    osg::ref_ptr<osgGA::OrbitManipulator> manipulator =
            dynamic_cast<osgGA::OrbitManipulator*>(_manager_cgi->getCameraManipulator());

    manipulator->setHeading(0.0);
    manipulator->setElevation(0.0);
}

void WidgetCGI::rightView()
{
    osg::ref_ptr<osgGA::OrbitManipulator> manipulator =
            dynamic_cast<osgGA::OrbitManipulator*>(_manager_cgi->getCameraManipulator());

    manipulator->setHeading(M_PI);
    manipulator->setElevation(0.0);
}

void WidgetCGI::setProject(std::shared_ptr<pro::Project> proj)
{
    _proj = proj;
    _manager_cgi->setProject(proj);

    osg::ref_ptr<osgGA::CameraManipulator> manipulator =
            _manager_cgi->getCameraManipulator();
    osg::ref_ptr<cgi::ManipulatorOrbit> manipulatorOrbit =
            dynamic_cast<cgi::ManipulatorOrbit*>(manipulator.get());

    if ( manipulatorOrbit.valid() )
    {
        setCameraManipulatorOrbit();
    }
    else
    {
        setCameraManipulatorTrack();
    }

    resetView();
}

void WidgetCGI::setAnimationTime(double time)
{
    _anim_time = time;
}

void WidgetCGI::setCameraManipulatorOrbit()
{
    _manager_cgi->setCameraManipulatorOrbit();
    getOsgViewer()->setCameraManipulator(_manager_cgi->getCameraManipulator());
}

void WidgetCGI::setCameraManipulatorTrack()
{
    _manager_cgi->setCameraManipulatorTrack();
    getOsgViewer()->setCameraManipulator(_manager_cgi->getCameraManipulator());
}

void WidgetCGI::resizeEvent(QResizeEvent* event)
{
    ////////////////////////////
    QWidget::resizeEvent(event);
    ////////////////////////////

    _manager_cgi->setWinHeight(event->size().height());
    _manager_cgi->setWinWidth(event->size().width());
}

void WidgetCGI::timerEvent(QTimerEvent* event)
{
    ///////////////////////////
    QWidget::timerEvent(event);
    ///////////////////////////

    if ( !_proj.expired() )
    {
        std::shared_ptr<pro::Project> proj = _proj.lock();
        proj->SetAnimationTime(_anim_time);
    }

    update();
    _manager_cgi->update();
}

QWidget* WidgetCGI::addViewWidget()
{
    createCameras();

    setSceneData(_manager_cgi->getNodeCGI());
    _camera_hud->addChild(_manager_cgi->getNodeHUD());

    addEventHandler(new osgViewer::StatsHandler);
    setKeyEventSetsDone(0);
    assignSceneDataToCameras();

    return _gwin->getGLWidget();
}

void WidgetCGI::createCameras()
{
    _camera_cgi = getOsgViewer()->getCamera();
    _camera_hud = new osg::Camera();

    _camera_cgi->setGraphicsContext(_gwin);
    _camera_hud->setGraphicsContext(_gwin);

    osg::GraphicsContext* context = _camera_cgi->getGraphicsContext();
    _camera_hud->setGraphicsContext(context);

    const osg::GraphicsContext::Traits* traits = _camera->getGraphicsContext()->getTraits();

    double w2h = static_cast<double>(traits->width)
               / static_cast<double>(traits->height);

    _camera_cgi->setClearColor(osg::Vec4(0.47, 0.71, 1.0, 1.0));
    //_camera_cgi->setClearColor(osg::Vec4(0.216, 0.216, 0.216, 1.0));
    _camera_cgi->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    _camera_cgi->setProjectionMatrixAsPerspective(30.0, w2h, 0.1, 1000000.0);

    _camera_hud->setProjectionMatrixAsOrtho2D(-CGI_HUD_Y_2 * w2h, CGI_HUD_Y_2 * w2h, -CGI_HUD_Y_2, CGI_HUD_Y_2);
    _camera_hud->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    _camera_hud->setViewMatrix(osg::Matrix::identity());
    _camera_hud->setClearMask(GL_DEPTH_BUFFER_BIT);
    _camera_hud->setRenderOrder(osg::Camera::POST_RENDER);
    _camera_hud->setAllowEventFocus(false);
    _camera_hud->setProjectionResizePolicy(osg::Camera::HORIZONTAL);
    _camera_hud->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));

    getOsgViewer()->addSlave(_camera_hud, false);
}

} // namespace gui
} // namespace mc
