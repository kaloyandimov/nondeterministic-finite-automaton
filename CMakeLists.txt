cmake_minimum_required(VERSION 3.23)

project(
    NondeterministicFiniteAutomaton 
    LANGUAGES CXX
)

set(CMAKE_CXX_EXTENSIONS OFF)

set(NFA_CORE_SOURCES
    "src/application/automaton_service.cpp"

    "src/automaton/automaton.cpp"
    "src/automaton/state.cpp"
    "src/automaton/transition.cpp"

    "src/controller/command.cpp"
    "src/controller/command_line_parser.cpp"
    "src/controller/command_registry.cpp"
    "src/controller/controller.cpp"
    "src/controller/default_commands.cpp"

    "src/export/automaton_dot_exporter.cpp"

    "src/persistence/automaton_repository.cpp"

    "src/regex/regex_parser.cpp"

    "src/serialization/automaton_serializer.cpp"
)

add_library(
    nfa_core 
    STATIC 
        ${NFA_CORE_SOURCES}
)

target_include_directories(
    nfa_core 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_features(
    nfa_core 
    PUBLIC 
        cxx_std_20
)

# Command-line application
add_executable(
    nfa 
    src/main.cpp
)

target_link_libraries(
    nfa 
    PRIVATE 
        nfa_core
)

# Tests
include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.15.0
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(Catch2)

    set(NFA_TEST_SOURCES
        "tests/automaton_tests.cpp"
        "tests/minimization_tests.cpp"
        "tests/regex_parser_tests.cpp"
        "tests/serialization_tests.cpp"
        "tests/controller_tests.cpp"
    )

    add_executable(
        nfa_tests
        ${NFA_TEST_SOURCES}
    )

    target_link_libraries(
        nfa_tests
        PRIVATE
            nfa_core
            Catch2::Catch2WithMain
    )

    list(
        APPEND CMAKE_MODULE_PATH
        "${catch2_SOURCE_DIR}/extras"
    )

    include(Catch)

    catch_discover_tests(nfa_tests)
endif()
