cmake_minimum_required(VERSION 3.10)

project(deepSeek)

find_package(Qt6 COMPONENTS Widgets Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB SOURCES_CPP ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADS_H ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_executable(${PROJECT_NAME} ${SOURCES_CPP} ${HEADS_H})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} Qt6::Widgets Qt6::Network)
