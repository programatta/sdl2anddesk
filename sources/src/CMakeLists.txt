cmake_minimum_required(VERSION 3.6)

project(MY_APP)

find_library(SDL2 SDL2)
find_library(SDL2_image SDL2_image)

if(ANDROID)
add_library(main SHARED)
  else()
add_executable(main)
  endif()

file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

target_sources(main PRIVATE ${SOURCES} ${HEADERS})

target_link_libraries(main SDL2 SDL2_image)
