add_subdirectory(cgi)
add_subdirectory(gui)
add_subdirectory(pro)

################################################################################

set(CPP_FILES
    main.cpp
)

#set(MOC_SRC_FILES)

set(RCC_SRC_FILES
    mc-sim-editor.qrc
)

set(SOURCES ${CPP_FILES} ${MOC_SRC_FILES} ${RCC_SRC_FILES})

################################################################################

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

set(LIBS
    ${OPENSCENEGRAPH_LIBRARIES}
    ${QT_LIBRARIES}
)

target_link_libraries(${PROJECT_NAME} 
    -Wl,--start-group
    cgi
    gui
    pro
    ${LIBS}
    -Wl,--end-group
)

