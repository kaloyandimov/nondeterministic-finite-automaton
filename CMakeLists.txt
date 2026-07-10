cmake_minimum_required(VERSION 3.23)

project(NondeterministicFiniteAutomaton)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(nfa ${SOURCES})

target_include_directories(nfa PRIVATE include)
