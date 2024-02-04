QT += core gui opengl xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

################################################################################

DESTDIR = $$PWD/../bin
TARGET = mcsim-editor

################################################################################

CONFIG += c++17 object_parallel_to_source

################################################################################

win32: RC_FILE = mcsim-editor.rc

################################################################################

win32: CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O2
unix:  CONFIG(release, debug|release): QMAKE_CXXFLAGS += -O2

#win32: QMAKE_CXXFLAGS += /Zc:wchar_t

win32: QMAKE_LFLAGS += /INCREMENTAL:NO

################################################################################

DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4):win32: DEFINES += USE_QT5

win32: DEFINES += \
    NOMINMAX \
    WIN32 \
    _WINDOWS \
    _CRT_SECURE_NO_DEPRECATE \
    _SCL_SECURE_NO_WARNINGS \
    _USE_MATH_DEFINES

win32: CONFIG(release, debug|release): DEFINES += NDEBUG
win32: CONFIG(debug, debug|release):   DEFINES += _DEBUG

unix: CONFIG(debug, debug|release):   DEFINES += _DEBUG

#CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT

unix: DEFINES += _LINUX_

DEFINES += MCSIM_EDITOR_EXTRA_COMPONENTS
DEFINES += MCSIM_EDITOR_MODEL_ASSEMBLER

################################################################################

INCLUDEPATH += ./ ../

win32: INCLUDEPATH += \
    $(OSG_ROOT)/include/

################################################################################

win32: LIBS += \
    -L$(OSG_ROOT)/lib \
    -lopengl32

win32:CONFIG(release, debug|release): LIBS += \
    -lOpenThreads \
    -losg \
    -losgDB \
    -losgGA \
    -losgParticle \
    -losgSim \
    -losgText \
    -losgUtil \
    -losgViewer \
    -losgWidget

win32:CONFIG(debug, debug|release):  LIBS += \
    -lOpenThreadsd \
    -losgd \
    -losgDBd \
    -losgGAd \
    -losgParticled \
    -losgSimd \
    -losgTextd \
    -losgUtild \
    -losgViewerd \
    -losgWidgetd

unix: LIBS += \
    -L/lib \
    -L/usr/lib \
    -L/usr/local/lib \
    -lOpenThreads \
    -losg \
    -losgDB \
    -losgGA \
    -losgParticle \
    -losgSim \
    -losgText \
    -losgUtil \
    -losgViewer \
    -losgWidget

LIBS += \
    -lmcutils

################################################################################

HEADERS += \
    $$PWD/defs.h \
    Result.h

SOURCES += \
    $$PWD/main.cpp

RESOURCES += \
    $$PWD/mc-sim-editor.qrc

################################################################################

include(cgi/cgi.pri)
include(gui/gui.pri)
include(pro/pro.pri)
include(sim/sim.pri)
