#ifndef CONTROLLER_CONTROLLER_HPP
#define CONTROLLER_CONTROLLER_HPP

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "automaton/automaton.hpp"
#include "controller/command.hpp"
#include "controller/parser.hpp"
#include "exception/command_not_found_exception.hpp"
#include "exception/custom_exception.hpp"
#include "exception/invalid_argument_count_exception.hpp"
#include "exception/invalid_expression_exception.hpp"
#include "exception/unbalanced_brackets_exception.hpp"
#include "export/automaton_dot_exporter.hpp"
#include "manager/file_manager.hpp"
#include "serialization/automaton_serializer.hpp"
#include "storage/storage.hpp"

class Controller {
 public:
    Controller(
        Storage<Automaton> as = {},
        FileManager<Automaton, AutomatonSerializer> fm = FileManager<Automaton, AutomatonSerializer>{},
        std::istream& = std::cin,
        std::ostream& = std::cout,
        std::ostream& = std::cerr
    );

    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(Controller&&) = delete;
    ~Controller() = default;

    void run();
    void stop();

 private:
    Storage<Automaton> storage_;
    FileManager<Automaton, AutomatonSerializer> fm_;
    AutomatonDotExporter exporter_;
    bool running_;

    std::istream& in_;
    std::ostream& out_;
    std::ostream& err_;

    bool basic(const std::string&) const;
    Command get_command(const Parser&);

    static std::vector<Command> commands;
    static bool init;
    static bool init_commands();

    static void register_command(const std::string&, const std::string&, const std::string&, ulong, const Function&);
};

#endif // CONTROLLER_CONTROLLER_HPP
