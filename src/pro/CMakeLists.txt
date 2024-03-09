set(MODULE_NAME pro)

################################################################################

set(CPP_FILES
    Animated.cpp
    Animation.cpp
    Assembly.cpp
    Box.cpp
    Component.cpp
    Components.cpp
    File.cpp
    FLOLS.cpp
    Group.cpp
    Keyframe.cpp
    LOD.cpp
    PAT.cpp
    Playback.cpp
    Project.cpp
    Rotor.cpp
    Switch.cpp
    Trans.cpp
)

################################################################################

add_library(${MODULE_NAME} STATIC ${CPP_FILES})

qt5_use_modules(${MODULE_NAME} Core Xml)
