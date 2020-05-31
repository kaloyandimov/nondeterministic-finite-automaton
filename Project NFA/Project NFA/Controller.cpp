//
//  Controller.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 30.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include <iostream>
#include "Controller.hpp"

std::vector<std::string> Controller::commands;
std::vector<std::string> Controller::usage;
std::vector<std::function<void(Arguments&, Controller&)>> Controller::functions;

Controller::Controller(const std::vector<NFA>& automata,
                       const std::string& file_name,
                       std::istream& in,
                       std::ostream& out):
    automata_{automata},
    file_{file_name},
    file_name_{file_name},
    in_{in},
    out_{out} {}

void
Controller::execute(const Command& command) {
    bool found = false;
    
    for (size_t i = 0; i < commands.size() && !found; ++i) {
        if (commands[i] == command.name()) {
            found = true;
            
            Arguments arguments{command.arguments()};
    
            functions[i](arguments, *this);
        }
    }
    
    if (command && !found) out_ << command.name() << ": not found" << std::endl;
}

bool
check(const std::string& name) {
    return name == "help" ||
           name == "open" ||
           name == "exit";
}

void
Controller::run() {
    is_running = true;
    Command command;
    
    while (is_running) {
        out_ << "> ";
        in_ >> command;
        
        if (file_.is_open() ||
            check(command.name())) {
            execute(command);
        }
        else {
            out_ << "First open a file\n";
        }
    }
}

void
Controller::stop() {
    is_running = false;
}

void
Controller::register_command(const std::string& name,
                             const std::string& help,
                             std::function<void(Arguments&,
                                                Controller&)> function) {
    commands.push_back(name);
    usage.push_back(help);
    functions.push_back(function);
}

bool
check_arguments_count(const Arguments& args,
                      size_t number,
                      std::ostream& out = std::cout) {
    if (args.size() < number) {
        out << "Not enough arguments. Needed " << number << '\n';
        
        return false;
    }
    else if (args.size() > number) {
        out << "Too many arguments. Needed " << number << '\n';
        
        return false;
    }
    
    return true;
}

bool
Controller::basic_commands = Controller::init_basic_commands();

bool
Controller::init_basic_commands() {
    register_command("list",
                     "list                  - list all automata's ids",
                     [](Arguments& args, Controller& ctrl) {
        std::for_each(ctrl.automata_.begin(),
                      ctrl.automata_.end(),
                      [&](const NFA& nfa) {
            ctrl.out_ << nfa.id() << ' ';
        });
        ctrl.out_ << '\n';
    });
    
    register_command("print",
                     "print <id>            - print all transitions of automaton with id",
                     [](Arguments& args, Controller& ctrl) {
        if (check_arguments_count(args, 1, ctrl.out_)) {
            auto automaton{std::find(ctrl.automata_.begin(),
                                     ctrl.automata_.end(),
                                     std::stoi(args[0]))};
            
            if (automaton != ctrl.automata_.end()) {
                ctrl.out_ << *automaton;
            }
            else ctrl.out_ << "Automaton with id " + args.at(0) + " not found" << std::endl;
        }
    });
    
    register_command("save",
                     "save <id> <filename>  - save automaton in file",
                     [](Arguments& args, Controller& ctrl) {
        if (check_arguments_count(args, 2, ctrl.out_)) {
            ctrl.file_.open(ctrl.file_name_, std::ios::out);
            
            if (ctrl.file_) {
                auto pos{std::find(ctrl.automata_.begin(),
                                   ctrl.automata_.end(),
                                   std::stoi(args[0]))};
                
                if (pos != ctrl.automata_.end()) {
                    ctrl.file_ << *pos;
                    
                    ctrl.out_ << "Automaton successfully saved\n";
                }
                else ctrl.out_ << "Automaton with id " + args.at(0) + " not found" << std::endl;
            }
            else {
                ctrl.out_ << "Error opening file " + args.at(1) + "\n";
            }
        }
    });
    
    register_command("empty",
                     "empty <id>            - check if automaton's language is empty",
                     [](Arguments& args, Controller& ctrl) {
        // TODO...
    });
    
    register_command("deterministic",
                     "deterministic <id>    - check if automaton is deterministic",
                     [](Arguments& args, Controller& ctrl) {
        // TODO...
    });
    
    register_command("recognize",
                     "recognize <id> <word> - check if automaton recognize the word",
                     [](Arguments& args, Controller& ctrl) {
        // TODO...
    });
    
    
    register_command("union",
                     "union <id1> <id2>     - find the union of two automata and print its id",
                     [](Arguments& args, Controller& ctrl) {
        if (check_arguments_count(args, 2, ctrl.out_)) {
            auto lhs{std::find(ctrl.automata_.begin(),
                               ctrl.automata_.end(),
                               std::stoi(args[0]))};
            
            auto rhs{std::find(ctrl.automata_.begin(),
                               ctrl.automata_.end(),
                               std::stoi(args[0]))};
            
            if (lhs != ctrl.automata_.end()) {
                if (rhs != ctrl.automata_.end()) {
                    NFA automaton{union_(*lhs, *rhs)};
                    automaton.set_id(ctrl.automata_.size());
                    
                    ctrl.out_ << "Automaton with ID "
                              << automaton.id()
                              << " created\n";
                    
                    ctrl.automata_.push_back(automaton);
                }
                else ctrl.out_ << "Automaton with id " +
                                  args.at(1) +
                                  " not found\n";
            }
            else ctrl.out_ << "Automaton with id " +
                              args.at(0) +
                              " not found\n";
        }
    });
    
    register_command("concat",
                     "conact <id1> <id2>    - find the concatenation of two automata and print its id",
                     [](Arguments& args, Controller& ctrl) {
        if (check_arguments_count(args, 2, ctrl.out_)) {
            auto lhs{std::find(ctrl.automata_.begin(),
                               ctrl.automata_.end(),
                               std::stoi(args[0]))};
            
            auto rhs{std::find(ctrl.automata_.begin(),
                               ctrl.automata_.end(),
                               std::stoi(args[0]))};
            
            if (lhs != ctrl.automata_.end()) {
                if (rhs != ctrl.automata_.end()) {
                    NFA automaton{concatenation_(*lhs, *rhs)};
                    automaton.set_id(ctrl.automata_.size());
                    
                    ctrl.out_ << "Automaton with ID "
                              << automaton.id()
                              << " created\n";
                    
                    ctrl.automata_.push_back(automaton);
                }
                else ctrl.out_ << "Automaton with id " +
                                  args.at(1) +
                                  " not found\n";
            }
            else ctrl.out_ << "Automaton with id " +
                              args.at(0) +
                              " not found\n";
        }
    });
    
    register_command("un",
                     "un <id>               - find positive closure of automaton and print its id",
                     [](Arguments& args, Controller& ctrl) {
        if (check_arguments_count(args, 1, ctrl.out_)) {
            auto pos{std::find(ctrl.automata_.begin(),
                               ctrl.automata_.end(),
                               std::stoi(args[0]))};
            
            if (pos != ctrl.automata_.end()) {
                NFA automaton{kleene_plus_(*pos)};
                automaton.set_id(ctrl.automata_.size());
                
                ctrl.out_ << "Automaton with ID " << automaton.id() << " created\n";
                
                ctrl.automata_.push_back(automaton);
            }
            else ctrl.out_ << "Automaton with id " + args.at(0) + " not found\n";
        }
    });
    
    register_command("reg",
                     "reg <regex>           - create new automaton from regular expresion and print its id",
                     [](Arguments& args, Controller& ctrl) {
        try {
            NFA automaton{parse(shunting_yard(args[0]))->evaluate()};
            automaton.set_id(ctrl.automata_.size());
            
            ctrl.out_ << "Automaton with ID " << automaton.id() << " created\n";
            
            ctrl.automata_.push_back(automaton);
        } catch (std::invalid_argument e) {
            ctrl.out_ << e.what() << '\n';
        }
    });
    
    register_command("open",
                     "open <filename>       - open file with specified name",
                     [](Arguments& args, Controller& ctrl) {
        if (check_arguments_count(args, 1, ctrl.out_)) {
            ctrl.file_.open(args[0]);
            
            if (ctrl.file_) {
                ctrl.out_ << "File opened successfully\n";
            }
            else {
                ctrl.out_ << "Error openning " + args[0] << "\n";
            }
        }
    });
    
    register_command("close",
                     "close                 - close file without saving",
                     [](Arguments& args, Controller& ctrl) {
        if (check_arguments_count(args, 0, ctrl.out_)) {
            ctrl.file_.close();
            
            if (ctrl.file_) {
                ctrl.out_ << "File closed successfully\n";;
            }
            else {
                ctrl.out_ << "Error closing\n";
            }
        }
    });
    
    register_command("save",
                     "save                  - save data to the same file",
                     [](Arguments& args, Controller& ctrl) {
        if (check_arguments_count(args, 0, ctrl.out_)) {
            ctrl.file_.open(ctrl.file_name_, std::ios::out);
            
            if (ctrl.file_) {
                std::for_each(ctrl.automata_.begin(),
                              ctrl.automata_.end(),
                              [&](const NFA& nfa) {
                    ctrl.file_ << nfa;
                });
            
                ctrl.out_ << "File successfully saved\n";
            }
            else {
                ctrl.out_ << "Error saving\n";
            }
        }
    });
    
    register_command("saveas",
                     "saveas <filename>     - save data to file with specified name",
                     [](Arguments& args, Controller& ctrl) {
        if (check_arguments_count(args, 1, ctrl.out_)) {
            ctrl.file_.open(args[0], std::ios::out);
            
            std::for_each(ctrl.automata_.begin(),
                          ctrl.automata_.end(),
                          [&](const NFA& nfa) {
                ctrl.file_ << nfa;
            });
            
            ctrl.out_ << "File successfully saved\n";
        }
    });
    
    register_command("exit",
                     "exit                  - exit the program",
                     [](Arguments& args, Controller& ctrl) {
        ctrl.stop();
          
        ctrl.out_ << "Exiting the program...\n";
    });
    
    
    register_command("help",
                     "help                  - display this list",
                     [](Arguments& args, Controller& ctrl) {
        if (check_arguments_count(args, 0, ctrl.out_)) {
            ctrl.out_ << "usage:\n\n";
            
            for (size_t i = 0; i < Controller::usage.size(); ++i) {
                ctrl.out_ << "  " << Controller::usage[i] << "\n\n";
            }
            
            ctrl.out_.flush();
        }
    });
    
    
    return true;
}


