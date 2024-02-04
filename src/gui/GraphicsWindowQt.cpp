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

#include <gui/GraphicsWindowQt.h>

#include <iostream>

#include <osg/Version>

#include <gui/KeyMap.h>

////////////////////////////////////////////////////////////////////////////////

namespace mc { namespace gui
{

////////////////////////////////////////////////////////////////////////////////

GraphicsWindowQt::GraphicsWindowQt(osg::GraphicsContext::Traits* traits)
{
    _traits = traits;

    osg::ref_ptr<WinData> winData
            = _traits.get() ? dynamic_cast<WinData*>(_traits->inheritedWindowData.get()) : nullptr;

    if ( !widget_ )
    {
        widget_ = winData.valid() ? winData->widget_ : nullptr;
    }

    ownsWidget_ = widget_ == nullptr;

    if ( !widget_ )
    {
        QGLWidget* shareWidget = nullptr;
        osg::ref_ptr<GraphicsWindowQt> sharedContext =
                dynamic_cast<GraphicsWindowQt*>(_traits->sharedContext.get());

        if ( sharedContext.valid() )
        {
            shareWidget = sharedContext->getGLWidget();
        }

        Qt::WindowFlags flags_tmp = Qt::Window | Qt::CustomizeWindowHint;
        if ( _traits->windowDecoration )
        {
            flags_tmp |= Qt::WindowTitleHint
                      |  Qt::WindowMinMaxButtonsHint
                      |  Qt::WindowSystemMenuHint;

#           if (QT_VERSION_CHECK(4, 5, 0) <= QT_VERSION)
            flags_tmp |= Qt::WindowCloseButtonHint;
#           endif
        }

        QGLFormat format(QGLFormat::defaultFormat());

        format.setAlphaBufferSize(_traits->alpha);
        format.setRedBufferSize(_traits->red);
        format.setGreenBufferSize(_traits->green);
        format.setBlueBufferSize(_traits->blue);
        format.setDepthBufferSize(_traits->depth);
        format.setStencilBufferSize(_traits->stencil);
        format.setSampleBuffers(_traits->sampleBuffers);
        format.setSamples(_traits->samples);

        format.setAlpha(_traits->alpha > 0);
        format.setDepth(_traits->depth > 0);
        format.setStencil(_traits->stencil > 0);

        format.setDoubleBuffer(_traits->doubleBuffer);
        format.setSwapInterval(_traits->vsync ? 1 : 0);
        format.setStereo(_traits->quadBufferStereo ? 1 : 0);

        widget_ = new GLWidget(format, nullptr, shareWidget, flags_tmp);
    }

    if ( ownsWidget_ )
    {
        widget_->setWindowTitle(_traits->windowName.c_str());
        widget_->move(_traits->x, _traits->y);

        if ( !_traits->supportsResize )
            widget_->setFixedSize( _traits->width, _traits->height );
        else
            widget_->resize( _traits->width, _traits->height );
    }

    widget_->setAutoBufferSwap(false);
    widget_->setMouseTracking(true);
    widget_->setFocusPolicy(Qt::WheelFocus);
    widget_->setGraphicsWindow(this);

    useCursor(_traits->useCursor);

    setState(new osg::State);
    getState()->setGraphicsContext(this);

    if ( _traits.valid() && _traits->sharedContext.valid() )
    {
        getState()->setContextID(_traits->sharedContext->getState()->getContextID());
        incrementContextIDUsageCount(getState()->getContextID());
    }
    else
    {
        getState()->setContextID(osg::GraphicsContext::createNewContextID());
    }

#   if OPENSCENEGRAPH_SOVERSION < 140
    getEventQueue()->syncWindowRectangleWithGraphcisContext();
#   else
    getEventQueue()->syncWindowRectangleWithGraphicsContext();
#   endif
}

////////////////////////////////////////////////////////////////////////////////

GraphicsWindowQt::~GraphicsWindowQt()
{
    if ( widget_ )
    {
        widget_->gwin_ = nullptr;
        widget_ = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::setWindowRectangleImplementation(int x, int y, int w, int h)
{
    if ( widget_ == nullptr ) return false;
    widget_->setGeometry(x, y, w, h);
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::getWindowRectangle(int& x, int& y, int& w, int& h)
{
    if ( widget_ )
    {
        const QRect &geom = widget_->geometry();

        x = geom.x();
        y = geom.y();
        w = geom.width();
        h = geom.height();
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::grabFocus()
{
    if ( widget_ ) widget_->setFocus(Qt::ActiveWindowFocusReason);
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::grabFocusIfPointerInWindow()
{
    if ( widget_->underMouse() ) widget_->setFocus(Qt::ActiveWindowFocusReason);
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::raiseWindow()
{
    if ( widget_ ) widget_->raise();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::useCursor(bool cursorOn)
{
    if ( widget_ )
    {
        _traits->useCursor = cursorOn;

        if ( !cursorOn )
            widget_->setCursor(Qt::BlankCursor);
        else
            widget_->setCursor(currentCursor_);
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::setCursor(MouseCursor cursor)
{
    if ( cursor == InheritCursor && widget_ )
    {
        widget_->unsetCursor();
    }

    switch ( cursor )
    {
        case NoCursor:          currentCursor_ = Qt::BlankCursor;     break;
        case RightArrowCursor:
        case LeftArrowCursor:   currentCursor_ = Qt::ArrowCursor;     break;
        case InfoCursor:        currentCursor_ = Qt::SizeAllCursor;   break;
        case DestroyCursor:     currentCursor_ = Qt::ForbiddenCursor; break;
        case HelpCursor:        currentCursor_ = Qt::WhatsThisCursor; break;
        case CycleCursor:       currentCursor_ = Qt::ForbiddenCursor; break;
        case SprayCursor:       currentCursor_ = Qt::SizeAllCursor;   break;
        case WaitCursor:        currentCursor_ = Qt::WaitCursor;      break;
        case TextCursor:        currentCursor_ = Qt::IBeamCursor;     break;
        case CrosshairCursor:   currentCursor_ = Qt::CrossCursor;     break;
        case HandCursor:        currentCursor_ = Qt::OpenHandCursor;  break;
        case UpDownCursor:      currentCursor_ = Qt::SizeVerCursor;   break;
        case LeftRightCursor:   currentCursor_ = Qt::SizeHorCursor;   break;
        case TopSideCursor:
        case BottomSideCursor:  currentCursor_ = Qt::UpArrowCursor;   break;
        case LeftSideCursor:
        case RightSideCursor:   currentCursor_ = Qt::SizeHorCursor;   break;
        case TopLeftCorner:     currentCursor_ = Qt::SizeBDiagCursor; break;
        case TopRightCorner:    currentCursor_ = Qt::SizeFDiagCursor; break;
        case BottomRightCorner: currentCursor_ = Qt::SizeBDiagCursor; break;
        case BottomLeftCorner:  currentCursor_ = Qt::SizeFDiagCursor; break;
        default: break;
    };

    if ( widget_ ) widget_->setCursor(currentCursor_);
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::valid() const
{
    return widget_ && widget_->isValid();
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::realizeImplementation()
{
    const QGLContext* savedContext = QGLContext::currentContext();

    if ( !valid() )
    {
        widget_->glInit();
    }

    realized_ = true;
    bool result = makeCurrent();
    realized_ = false;

    if ( !result )
    {
        if ( savedContext )
        {
            const_cast<QGLContext*>(savedContext)->makeCurrent();
        }

        std::cerr << "Window realize: Can make context current." << std::endl;

        return false;
    }

    realized_ = true;

#   if OPENSCENEGRAPH_SOVERSION < 140
    //getEventQueue()->syncWindowRectangleWithGraphcisContext();
    getEventQueue()->syncWindowRectangleWithGraphicsContext();
#   else
    getEventQueue()->syncWindowRectangleWithGraphicsContext();
#   endif

    if ( !releaseContext() )
    {
        std::cerr << "Window realize: Can not release context." << std::endl;
    }

    if ( savedContext )
    {
        const_cast<QGLContext*>(savedContext)->makeCurrent();
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::isRealizedImplementation() const
{
    return realized_;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::closeImplementation()
{
    if ( widget_ ) widget_->close();
    realized_ = false;
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::makeCurrentImplementation()
{
    if ( widget_->getNumDeferredEvents() > 0 )
    {
        widget_->processDeferredEvents();
    }

    widget_->makeCurrent();

    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::releaseContextImplementation()
{
    widget_->doneCurrent();
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::swapBuffersImplementation()
{
    widget_->swapBuffers();

    if ( widget_->getNumDeferredEvents() > 0 )
    {
        widget_->processDeferredEvents();
    }

    if ( QGLContext::currentContext() != widget_->context() )
    {
        widget_->makeCurrent();
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::runOperations()
{
    if ( widget_->getNumDeferredEvents() > 0 )
    {
        widget_->processDeferredEvents();
    }

    if ( QGLContext::currentContext() != widget_->context() )
    {
        widget_->makeCurrent();
    }

    GraphicsWindow::runOperations();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::requestWarpPointer(float x, float y)
{
    if ( widget_ )
    {
        QCursor::setPos(widget_->mapToGlobal(QPoint(static_cast<int>(x), static_cast<int>(y))));
    }
}

////////////////////////////////////////////////////////////////////////////////

GraphicsWindowQt::GLWidget::GLWidget(const QGLFormat& format,
                                     QWidget* parent, const QGLWidget* shareWidget,
                                     Qt::WindowFlags flags)
    : QGLWidget(format, parent, shareWidget, flags)
    , gwin_( nullptr )
{}

////////////////////////////////////////////////////////////////////////////////

GraphicsWindowQt::GLWidget::~GLWidget()
{
    if ( gwin_ )
    {
        gwin_->close();
        gwin_->widget_ = nullptr;
        gwin_ = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::setKeyboardModifiers(QInputEvent* event)
{
    int modkey = event->modifiers() & ( Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier );

    unsigned int mask = 0;

    if ( modkey & Qt::ShiftModifier   ) mask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    if ( modkey & Qt::ControlModifier ) mask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    if ( modkey & Qt::AltModifier     ) mask |= osgGA::GUIEventAdapter::MODKEY_ALT;

    gwin_->getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}

////////////////////////////////////////////////////////////////////////////////

bool GraphicsWindowQt::GLWidget::event(QEvent* event)
{
    if ( event->type() == QEvent::Hide )
    {
        enqueueDeferredEvent(QEvent::Hide, QEvent::Show);
        return true;
    }
    else if ( event->type() == QEvent::Show )
    {
        enqueueDeferredEvent(QEvent::Show, QEvent::Hide);
        return true;
    }
    else
    if ( event->type() == QEvent::ParentChange )
    {
        enqueueDeferredEvent(QEvent::ParentChange);
        return true;
    }

    return QGLWidget::event(event);
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::keyPressEvent(QKeyEvent* event)
{
    setKeyboardModifiers(event);
    gwin_->getEventQueue()->keyPress(KeyMap::remapOSG(event->key()));
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::keyReleaseEvent(QKeyEvent* event)
{
    if( event->isAutoRepeat() )
    {
        event->ignore();
    }
    else
    {
        setKeyboardModifiers(event);
        gwin_->getEventQueue()->keyRelease(KeyMap::remapOSG(event->key()));
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::mousePressEvent(QMouseEvent* event)
{
    int button = 0;

    switch ( event->button() )
    {
        case Qt::LeftButton:   button = 1; break;
        case Qt::MiddleButton: button = 2; break;
        case Qt::RightButton:  button = 3; break;
        case Qt::NoButton:     button = 0; break;
        default:               button = 0; break;
    }

    setKeyboardModifiers(event);
    gwin_->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
    int button = 0;

    switch ( event->button() )
    {
        case Qt::LeftButton:   button = 1; break;
        case Qt::MiddleButton: button = 2; break;
        case Qt::RightButton:  button = 3; break;
        case Qt::NoButton:     button = 0; break;
        default:               button = 0; break;
    }

    setKeyboardModifiers(event);
    gwin_->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    int button = 0;

    switch ( event->button() )
    {
        case Qt::LeftButton:   button = 1; break;
        case Qt::MiddleButton: button = 2; break;
        case Qt::RightButton:  button = 3; break;
        case Qt::NoButton:     button = 0; break;
        default:               button = 0; break;
    }

    setKeyboardModifiers(event);
    gwin_->getEventQueue()->mouseDoubleButtonPress(event->x(), event->y(), button);
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    setKeyboardModifiers(event);
    gwin_->getEventQueue()->mouseMotion(event->x(), event->y());
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::moveEvent(QMoveEvent* event)
{
    const QPoint& pos = event->pos();

    gwin_->resized(pos.x(), pos.y(), width(), height());
    gwin_->getEventQueue()->windowResize(pos.x(), pos.y(), width(), height());
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::resizeEvent(QResizeEvent* event)
{
    const QSize& size = event->size();

    gwin_->resized(x(), y(), size.width(), size.height());
    gwin_->getEventQueue()->windowResize(x(), y(), size.width(), size.height());
    gwin_->requestRedraw();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::wheelEvent(QWheelEvent* event)
{
    setKeyboardModifiers( event );

    if ( event->angleDelta().y() != 0 )
    {
        gwin_->getEventQueue()->mouseScroll(event->angleDelta().y() > 0 ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN);
    }
    else
    {
        gwin_->getEventQueue()->mouseScroll(event->angleDelta().x() > 0 ? osgGA::GUIEventAdapter::SCROLL_LEFT : osgGA::GUIEventAdapter::SCROLL_RIGHT);
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::glDraw()
{
    gwin_->requestRedraw();
}

////////////////////////////////////////////////////////////////////////////////

int GraphicsWindowQt::GLWidget::getNumDeferredEvents()
{
    QMutexLocker lock(&deferredEventQueueMutex_);
    return deferredEventQueue_.count();
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::enqueueDeferredEvent(QEvent::Type eventType,
                                                      QEvent::Type removeEventType)
{
    QMutexLocker lock(&deferredEventQueueMutex_);

    if ( removeEventType != QEvent::None )
    {
        if ( deferredEventQueue_.removeOne(removeEventType) )
        {
            eventCompressor_.remove(eventType);
        }
    }

    if ( eventCompressor_.find(eventType) == eventCompressor_.end() )
    {
        deferredEventQueue_.enqueue(eventType);
        eventCompressor_.insert(eventType);
    }
}

////////////////////////////////////////////////////////////////////////////////

void GraphicsWindowQt::GLWidget::processDeferredEvents()
{
    QQueue<QEvent::Type> deferredEventQueueCopy;

    QMutexLocker lock(&deferredEventQueueMutex_);

    deferredEventQueueCopy = deferredEventQueue_;

    eventCompressor_.clear();
    deferredEventQueue_.clear();

    while (!deferredEventQueueCopy.isEmpty())
    {
        QEvent event(deferredEventQueueCopy.dequeue());
        QGLWidget::event(&event);
    }
}

////////////////////////////////////////////////////////////////////////////////

} // namespace gui
} // namespace mc
