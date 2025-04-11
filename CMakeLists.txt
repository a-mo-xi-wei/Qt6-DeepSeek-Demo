cmake_minimum_required(VERSION 3.10)

project(deepSeek)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB SOURCES_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

file(GLOB HEADS_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(FILES
        ${SOURCES_CPP}
        ${HEADS_H}
        widget.ui
)

add_executable(${PROJECT_NAME} ${FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network)
