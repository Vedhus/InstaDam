cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 11)
project(quazip)
set (quazip_VERSION_MAJOR 1)
set (quazip_VERSION_MINOR 0)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

file(GLOB SRCS "*.c" "*.cpp")
file(GLOB PUBLIC_HEADERS "*.h")

# Must be added to enable export macro
ADD_DEFINITIONS(-DQUAZIP_BUILD)

qt_wrap_cpp(MOC_SRCS ${PUBLIC_HEADERS})
set(SRCS ${SRCS} ${MOC_SRCS})

add_library(quazip SHARED ${SRCS})

# Link against ZLIB_LIBRARIES if needed (on Windows this variable is empty)
target_link_libraries(quazip Qt5::Widgets Qt5::Gui Qt5::Core ${ZLIB_LIBRARIES})

#install(FILES ${PUBLIC_HEADERS} DESTINATION include/quazip${QUAZIP_LIB_VERSION_SUFFIX})
#install(TARGETS quazip LIBRARY DESTINATION ..)
