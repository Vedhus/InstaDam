cmake_minimum_required (VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 11)
project(InstaDam)
set (InstaDam_VERSION_MAJOR 1)
set (InstaDam_VERSION_MINOR 0)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)


add_subdirectory(quazip)
add_subdirectory(Selector)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Network CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

# Find the OpenCV
find_package(OpenCV CONFIG REQUIRED)

set(SRCS
    app/main.cpp
    app/instadam.cpp
    app/project.cpp
    app/maskviewer.cpp
    app/photoviewer.cpp
    app/pixmapops.cpp
    app/filterproperty.cpp
    app/picpushbutton.cpp
    app/maskobjects.cpp
    app/filtercontrols.cpp
    app/fslider.cpp
    app/newproject.cpp
    app/labelButton.cpp
    app/register.cpp
    app/startingwidget.cpp
    app/login.cpp
    app/projectlist.cpp
    app/imagelist.cpp
    app/serverprojectname.cpp
    app/enumconstants.cpp
    app/addusertoproject.cpp
    app/userprivilege.cpp
    app/chooselabeldialog.cpp
    app/commands.cpp
    app/instadam.ui
    app/newproject.ui
    Selector/blankFrame.ui
    Selector/polygonSelect.ui
    Selector/freeSelect.ui
    app/labeldialog.ui
    app/register.ui
    app/startingwidget.ui
    app/login.ui
    app/projectlist.ui
    app/imagelist.ui
    app/projectDialog.ui
    app/serverprojectname.ui
    app/addusertoproject.ui
    app/userprivilege.ui
    app/instadam.h
    app/project.h
    app/photoviewer.h
    app/maskviewer.h
    app/pixmapops.h
    app/filterproperty.h
    app/picpushbutton.h
    app/maskobjects.h
    app/filtercontrols.h
    app/filterproperty.h
    app/fslider.h
    app/newproject.h
    app/labelButton.h
    app/register.h
    app/startingwidget.h
    app/login.h
    app/projectlist.h
    app/imagelist.h
    app/serverprojectname.h
    app/enumconstants.h
    app/addusertoproject.h
    app/userprivilege.h
    app/chooselabeldialog.h
    app/commands.h
)
set(RUNTIME_OUTPUT_NAME InstaDam)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

include_directories(${CMAKE_HOME_DIRECTORY}/Selector ${CMAKE_HOME_DIRECTORY}/quazip ${CMAKE_HOME_DIRECTORY}/app)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIR})
link_directories(${CMAKE_HOME_DIRECTORY}/lib ${OpenCV_LINK_DIRECTORIES})
add_executable(InstaDam ${SRCS})
target_link_libraries(InstaDam Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Network selector quazip ${OpenCV_LIBRARIES})
