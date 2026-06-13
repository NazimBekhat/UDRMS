cmake_minimum_required(VERSION 3.19)
project(UDRMS LANGUAGES CXX)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()
qt_add_executable(UDRMS
    WIN32 MACOSX_BUNDLE
    main.cpp
    ui/src/mainwindow.cpp
    ui/include/mainwindow.h
    ui/src/mainwindow.ui
)
target_include_directories(UDRMS PRIVATE
    core/include
    ui/include
)
target_link_libraries(UDRMS
    PRIVATE
        Qt6::Core
        Qt6::Widgets
)
