set(MODULE_NAME gui)

################################################################################

set(CPP_FILES
    FormAnim.cpp
    FormComp.cpp
    FormScene.cpp
    GraphicsWindowQt.cpp
    KeyMap.cpp
    MainWindow.cpp
    MoveAction.cpp
    RecentAction.cpp
    TreeScene.cpp
    Utils.cpp
    WidgetCGI.cpp
    WidgetOSG.cpp
)

set(MOC_SRC_FILES
    FormAnim.h
    FormComp.h
    FormScene.h
    MainWindow.h
    MoveAction.h
    RecentAction.h
    TreeScene.h
    WidgetCGI.h
    WidgetOSG.h
)

set(UIC_SRC_FILES
    FormAnim.ui
    FormComp.ui
    FormScene.ui
    MainWindow.ui
)

################################################################################

add_library(${MODULE_NAME} STATIC ${CPP_FILES} ${MOC_SRC_FILES} ${UIC_SRC_FILES})

################################################################################

qt5_use_modules(${MODULE_NAME} Widgets OpenGL Xml)
