//
//  Controller.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Controller_hpp
#define Controller_hpp

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Automaton.hpp"
#include "Command.hpp"
#include "CommandNotFoundException.hpp"
#include "InvalidArgumentCountException.hpp"
#include "InvalidExpressionException.hpp"
#include "Parser.hpp"
#include "UnbalancedBracketsException.hpp"

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

#endif /* Controller_hpp */
