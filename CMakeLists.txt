cmake_minimum_required(VERSION 3.10)

project(plotter)

include(FetchContent) # If not included already

FetchContent_Declare(sciplot_content
  GIT_REPOSITORY https://github.com/sciplot/sciplot.git
  GIT_TAG master)

FetchContent_GetProperties(sciplot_content)
if(NOT sciplot_content_POPULATED)
  FetchContent_Populate(sciplot_content)
endif()

set(CMAKE_CXX_STANDARD 17)
include_directories(${sciplot_content_SOURCE_DIR})

add_executable(main main.cpp)