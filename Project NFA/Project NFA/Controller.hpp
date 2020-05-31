//
//  Controller.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 30.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef Controller_hpp
#define Controller_hpp

#include <fstream>
#include <functional>
#include "Command.hpp"
#include "ParserFunctions.hpp"

class Controller {
 public:
    Controller(const std::vector<NFA>& = {},
               const std::string& = {},
               std::istream& = std::cin,
               std::ostream& = std::cout);
    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;
    ~Controller() = default;
    
    void run();
    void stop();
    
 private:
    std::vector<NFA> automata_;
    std::fstream file_;
    std::string file_name_;
    std::istream& in_;
    std::ostream& out_;
    bool is_running;
    
    void execute(const Command&);
    
    static std::vector<std::string> commands;
    static std::vector<std::string> usage;
    static std::vector<std::function<void(Arguments&,
                                          Controller&)>> functions;
    
    static bool basic_commands;
    static bool init_basic_commands();
    
    static void register_command(const std::string&,
                                 const std::string&,
                                 std::function<void(Arguments&,
                                                    Controller&)>);
};

#endif /* Controller_hpp */
