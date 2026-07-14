cmake_minimum_required(VERSION 3.23)

project(NondeterministicFiniteAutomaton CXX)

add_executable(nfa 
    src/main.cpp
    src/automaton/automaton.cpp
    src/automaton/state.cpp
    src/automaton/transition.cpp
    src/controller/command.cpp
    src/controller/controller.cpp
    src/controller/parser.cpp
    src/exception/command_not_found_exception.cpp
    src/exception/custom_exception.cpp
    src/exception/invalid_argument_count_exception.cpp
    src/exception/invalid_expression_exception.cpp
    src/exception/invalid_symbol_exception.cpp
    src/exception/unbalanced_brackets_exception.cpp
    src/export/automaton_dot_exporter.cpp
    src/serialization/automaton_serializer.cpp
)

target_compile_features(nfa
    PRIVATE cxx_std_20
)

target_include_directories(nfa
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
