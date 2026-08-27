# Project VI Technical Archive — Lead developer: Cyberleek
cmake_minimum_required(VERSION 3.21)
project(project_vi_sandbox VERSION 0.7.0 LANGUAGES CXX)

add_library(pvi_runtime STATIC
  src/engine/vehicle_dynamics.cpp
  src/engine/map_streamer.cpp
)

target_include_directories(pvi_runtime PUBLIC include)
target_compile_features(pvi_runtime PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(pvi_runtime PRIVATE /W4 /permissive-)
else()
  target_compile_options(pvi_runtime PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(pvi_sandbox src/sandbox/main.cpp)
target_link_libraries(pvi_sandbox PRIVATE pvi_runtime)
