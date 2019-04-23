cmake_minimum_required(VERSION 3.1.0)
set(CMAKE_CXX_STANDARD 11)
project(quazip)
set (quazip_VERSION_MAJOR 1)
set (quazip_VERSION_MINOR 0)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets 5.9 CONFIG REQUIRED)
find_package(Qt5Core 5.9 CONFIG REQUIRED)
find_package(Qt5Gui 5.9 CONFIG REQUIRED)

file(GLOB SRCS "*.c" "*.cpp")
file(GLOB PUBLIC_HEADERS "*.h")

# Must be added to enable export macro
ADD_DEFINITIONS(-DQUAZIP_BUILD)

qt_wrap_cpp(MOC_SRCS ${PUBLIC_HEADERS})
set(SRCS ${SRCS} ${MOC_SRCS})

add_library(quazip SHARED ${SRCS})

# Link against ZLIB_LIBRARIES if needed (on Windows this variable is empty)
target_link_libraries(quazip Qt5::Widgets Qt5::Gui Qt5::Core ${ZLIB_LIBRARIES})

install(TARGETS quazip
        LIBRARY DESTINATION lib)