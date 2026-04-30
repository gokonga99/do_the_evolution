cmake_minimum_required(VERSION 3.16)
project(EvolutionSim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Find SFML first
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

file(GLOB SOURCES "src/*.cpp")

add_executable(sim ${SOURCES})

target_link_libraries(sim sfml-graphics sfml-window sfml-system)