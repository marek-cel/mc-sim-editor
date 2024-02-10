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
#ifndef MC_SIM_EDITOR_GUI_GRAPHICSWINDOWQT_H_
#define MC_SIM_EDITOR_GUI_GRAPHICSWINDOWQT_H_

#include <osg/Version>

#include <osgViewer/GraphicsWindow>

#include <QEvent>
#include <QGLWidget>
#include <QInputEvent>
#include <QMutex>
#include <QQueue>
#include <QSet>

#include <defs.h>

namespace mc {
namespace gui {

/** */
class GraphicsWindowQt : public osgViewer::GraphicsWindow
{
    class GLWidget;

    friend class GLWidget;

public:

    struct WinData : public osg::Referenced
    {
        WinData(GLWidget* widget = nullptr, QWidget* parent = nullptr)
            : widget_(widget)
            , parent_(parent)
        {}

        GLWidget* widget_ = nullptr;    ///<
        QWidget*  parent_ = nullptr;    ///<
    };

    /**
     * @brief Constructor.
     * @param traits
     */
    explicit GraphicsWindowQt(osg::GraphicsContext::Traits* traits);

    /** @brief Destructor. */
    virtual ~GraphicsWindowQt();

    virtual bool setWindowRectangleImplementation(int x, int y, int w, int h);
    virtual void getWindowRectangle(int& x, int& y, int& w, int& h);
    virtual void grabFocus();
    virtual void grabFocusIfPointerInWindow();
    virtual void raiseWindow();
    virtual void useCursor(bool cursorOn);
    virtual void setCursor(MouseCursor cursor);

    virtual bool valid() const;
    virtual bool realizeImplementation();
    virtual bool isRealizedImplementation() const;
    virtual void closeImplementation();
    virtual bool makeCurrentImplementation();
    virtual bool releaseContextImplementation();
    virtual void swapBuffersImplementation();
    virtual void runOperations();

    virtual void requestWarpPointer(float x, float y);

    inline       GLWidget* getGLWidget()       { return widget_; }
    inline const GLWidget* getGLWidget() const { return widget_; }

private:

    class GLWidget : public QGLWidget
    {
        friend class GraphicsWindowQt;

    public:

        GLWidget(const QGLFormat &format,
                 QWidget* parent = nullptr, const QGLWidget* shareWidget = nullptr,
                 Qt::WindowFlags flags = Qt::Widget);

        virtual ~GLWidget();

        inline       GraphicsWindowQt* getGraphicsWindow()       { return gwin_; }
        inline const GraphicsWindowQt* getGraphicsWindow() const { return gwin_; }

        inline void setGraphicsWindow(GraphicsWindowQt* gwin) { gwin_ = gwin; }

        void setKeyboardModifiers(QInputEvent* event);

    protected:

        virtual bool event(QEvent* event);

        virtual void keyPressEvent(QKeyEvent* event);
        virtual void keyReleaseEvent(QKeyEvent* event);

        virtual void mousePressEvent(QMouseEvent* event);
        virtual void mouseReleaseEvent(QMouseEvent* event);
        virtual void mouseDoubleClickEvent(QMouseEvent* event);
        virtual void mouseMoveEvent(QMouseEvent* event);

        virtual void moveEvent(QMoveEvent* event);

        virtual void resizeEvent(QResizeEvent* event);

        virtual void wheelEvent(QWheelEvent* event);

        virtual void glDraw();

    private:

        GraphicsWindowQt* gwin_;                        ///<

        QMutex deferredEventQueueMutex_;                ///<

        QQueue <QEvent::Type> deferredEventQueue_;      ///<
        QSet   <QEvent::Type> eventCompressor_;         ///<

        int getNumDeferredEvents();

        void enqueueDeferredEvent(QEvent::Type eventType,
                                  QEvent::Type removeEventType = QEvent::None);

        void processDeferredEvents();
    };

    GLWidget* widget_ = nullptr;    ///<
    QCursor currentCursor_;         ///<

    bool ownsWidget_ = false;       ///<
    bool realized_   = false;       ///<
};

} // namespace gui
} // namespace mc

#endif // MC_SIM_EDITOR_GUI_GRAPHICSWINDOWQT_H_
