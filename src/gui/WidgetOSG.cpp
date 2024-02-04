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

#include <gui/WidgetOSG.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui
{

////////////////////////////////////////////////////////////////////////////////

WidgetOSG::WidgetOSG(QWidget* parent)
    : QWidget(parent)
{
    setThreadingModel(osgViewer::ViewerBase::SingleThreaded);
    //setThreadingModel(osgViewer::ViewerBase::ThreadPerContext);

    gwin_ = createGraphicsWindow(x(), y(), width(), height());
}

////////////////////////////////////////////////////////////////////////////////

WidgetOSG::~WidgetOSG() {}

////////////////////////////////////////////////////////////////////////////////

osgViewer::Viewer* WidgetOSG::getOsgViewer()
{
    return this;
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOSG::setSceneData(osg::Node* node)
{
    osgViewer::Viewer::setSceneData(node);
}

////////////////////////////////////////////////////////////////////////////////

void WidgetOSG::paintEvent(QPaintEvent* event)
{
    ///////////////////////////
    QWidget::paintEvent(event);
    ///////////////////////////

    frame();
}

////////////////////////////////////////////////////////////////////////////////

osg::ref_ptr<GraphicsWindowQt> WidgetOSG::createGraphicsWindow(int x, int y, int w, int h)
{
    osg::DisplaySettings* displaySettings = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();

    traits->windowName       = "";
    traits->windowDecoration = false;
    traits->x                = x;
    traits->y                = y;
    traits->width            = w;
    traits->height           = h;
    traits->doubleBuffer     = true;
    traits->alpha            = displaySettings->getMinimumNumAlphaBits();
    traits->stencil          = displaySettings->getMinimumNumStencilBits();
    traits->sampleBuffers    = displaySettings->getMultiSamples();
    traits->samples          = 4;
    traits->vsync            = true;

    osg::ref_ptr<GraphicsWindowQt> graphicsWindow = new GraphicsWindowQt(traits.get());

    return graphicsWindow;
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gui
} // namespace mc
