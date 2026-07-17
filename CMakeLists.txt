cmake_minimum_required(VERSION 3.23)

project(NondeterministicFiniteAutomaton CXX)

add_executable(nfa 
    "src/main.cpp"
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

target_compile_features(nfa
    PRIVATE cxx_std_20
)

target_include_directories(nfa
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
