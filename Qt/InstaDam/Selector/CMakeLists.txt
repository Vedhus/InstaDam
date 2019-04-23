cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 11)
project(Selector)
set (Selector_VERSION_MAJOR 1)
set (Selector_VERSION_MINOR 0)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets 5.9 CONFIG REQUIRED)
find_package(Qt5Core 5.9 CONFIG REQUIRED)
find_package(Qt5Gui 5.9 CONFIG REQUIRED)

set(SRCS
    polygonSelect.cpp
    ellipseSelect.cpp
    photoScene.cpp
    rectangleSelect.cpp
    selectItem.cpp
    freeDrawSelect.cpp
    boxbasedselector.cpp
    label.cpp
    freeDrawErase.cpp
    polygonSelect.h
    ellipseSelect.h
    photoScene.h
    rectangleSelect.h
    selectItem.h
    freeDrawSelect.h
    boxbasedselector.h
    label.h
    freeDrawErase.h
)

add_library(selector SHARED ${SRCS})
target_link_libraries(selector Qt5::Widgets)

install(TARGETS selector
        LIBRARY DESTINATION lib)