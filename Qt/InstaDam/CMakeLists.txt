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
find_package(OpenCV 4 CONFIG REQUIRED)

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
    app/blankFrame.ui
    app/polygonSelect.ui
    app/freeSelect.ui
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
set(CMAKE_RUNTIME_OUTPUT_NAME InstaDam)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_BINARY_DIR}/Selector ${CMAKE_BINARY_DIR}/quazip ${CMAKE_BINARY_DIR}/app)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/app ${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIR})
link_directories(${CMAKE_BINARY_DIR}/lib ${OpenCV_LINK_DIRECTORIES})
add_executable(InstaDam ${SRCS})
target_link_libraries(InstaDam Qt5::Widgets Qt5::Gui Qt5::Core Qt5::Network selector quazip ${OpenCV_LIBRARIES})

install(TARGETS InstaDam
        RUNTIME DESTINATION bin)
# report what was found
message("  ")
message("Using Qt version:     ${Qt5Core_VERSION}")
message("Using OpenCV version: ${OpenCV_VERSION}")
message("  ")
message("Install prefix: ${CMAKE_INSTALL_PREFIX}")
message("  Binary path:  ${CMAKE_INSTALL_PREFIX}/bin")
message("  Library path: ${CMAKE_INSTALL_PREFIX}/${CMAKE_SHARED_LIBRARY_PREFIX}")
message("  ")
