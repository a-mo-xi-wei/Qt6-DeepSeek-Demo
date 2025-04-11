cmake_minimum_required(VERSION 3.10)

project(deepSeek)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
    src/main.cpp
    src/widget.cpp
        src/Chat.cpp
    src/widget.ui
)

set(HEADS
    include/widget.h
        include/Chat.h
)

#加载头文件到环境中
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(deepSeek ${SOURCES} ${HEADS})

target_link_libraries(deepSeek Qt6::Widgets Qt6::Network)
