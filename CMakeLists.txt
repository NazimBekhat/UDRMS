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
    ui/src/loginscreen.cpp
    ui/include/loginscreen.h
    ui/src/loginscreen.ui
    core/src/User.cpp
    core/src/Student.cpp
    core/src/Staff.cpp
    core/src/Administrator.cpp
    core/src/SystemAdmin.cpp
    core/src/Room.cpp
    core/src/SingleRoom.cpp
    core/src/DoubleRoom.cpp
    core/src/SharedRoom.cpp
    core/src/Restaurant.cpp
    core/src/Dormitory.cpp
    core/src/University.cpp
    core/src/UDRMSException.cpp
    core/src/Exceptions.cpp
    ui/include/dashboardscreen.h
    ui/src/dashboardscreen.cpp
    ui/src/dashboardscreen.ui
    ui/include/roommanagementscreen.h 
    ui/src/roommanagementscreen.cpp 
    ui/src/roommanagementscreen.ui
    ui/include/createuserscreen.h
     ui/src/createuserscreen.cpp
      ui/src/createuserscreen.ui
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