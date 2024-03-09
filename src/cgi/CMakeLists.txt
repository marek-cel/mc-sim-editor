set(MODULE_NAME cgi)

################################################################################

set(CPP_FILES
    Builder.cpp
    CGI.cpp
    Colors.cpp
    Component.cpp
    Geometry.cpp
    Grid.cpp
    HUD.cpp
    Manager.cpp
    ManipulatorOrbit.cpp
    ManipulatorTrack.cpp
    Model.cpp
    Models.cpp
    Textures.cpp
    ViewGizmo.cpp
)

################################################################################

add_library(${MODULE_NAME} STATIC ${CPP_FILES})

qt5_use_modules(${MODULE_NAME} Core Xml)
