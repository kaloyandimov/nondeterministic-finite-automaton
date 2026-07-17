#ifndef CONTROLLER_CONTROLLER_HPP
#define CONTROLLER_CONTROLLER_HPP

#include <iostream>

#include "application/automaton_service.hpp"
#include "controller/command_registry.hpp"
#include "controller/default_commands.hpp"

class Controller {
 public:
    Controller(
        AutomatonService service = {},
        CommandRegistry commands = create_default_command_registry(),
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

 private:
    AutomatonService service_;
    CommandRegistry commands_;
    bool running_{false};
    std::istream& in_;
    std::ostream& out_;
    std::ostream& err_;
};

#endif // CONTROLLER_CONTROLLER_HPP
