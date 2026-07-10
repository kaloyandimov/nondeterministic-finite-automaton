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

class Controller {
   public:
    Controller(std::istream& = std::cin, std::ostream& = std::cout, std::ostream& = std::cerr);
    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(Controller&&) = delete;
    ~Controller() = default;

    void run();
    void stop();

   private:
    std::vector<Automaton> automata_;
    std::filesystem::path path_;
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

#endif /* CONTROLLER_CONTROLLER_HPP */
