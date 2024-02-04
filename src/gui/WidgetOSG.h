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
#ifndef MC_SIM_EDITOR_GUI_WIDGETOSG_H_
#define MC_SIM_EDITOR_GUI_WIDGETOSG_H_

////////////////////////////////////////////////////////////////////////////////

#include <QDateTime>
#include <QGridLayout>
#include <QWidget>

#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>

#include <defs.h>

#include <gui/GraphicsWindowQt.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui
{

/**
 * @brief OSG common widget class.
 */
class WidgetOSG : public QWidget, public osgViewer::Viewer
{
    Q_OBJECT

public:

    /** @brief Constructor. */
    explicit WidgetOSG(QWidget* parent = nullptr);

    /** @brief Destructor. */
    virtual ~WidgetOSG();

    virtual osgViewer::Viewer* getOsgViewer();

    virtual void setSceneData(osg::Node* node);

protected:

    QGridLayout* layout_ = nullptr;         ///<

    osg::ref_ptr<GraphicsWindowQt> gwin_;   ///<

    bool initialized_ = true;               ///<

    /** */
    virtual void paintEvent(QPaintEvent* event);

    /** */
    osg::ref_ptr<GraphicsWindowQt> createGraphicsWindow(int x, int y, int w, int h);
};

} // namespace gui
} // namespace mc

////////////////////////////////////////////////////////////////////////////////

#endif // MC_SIM_EDITOR_GUI_WIDGETOSG_H_
