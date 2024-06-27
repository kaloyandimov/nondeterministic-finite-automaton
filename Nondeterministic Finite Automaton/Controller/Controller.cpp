//
//  Controller.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#include "Controller.hpp"

std::vector<Command> Controller::commands;

Controller::Controller(std::istream& in, std::ostream& out, std::ostream& err):
    in_{in}, out_{out}, err_{err}, running_{false} {}


bool Controller::basic(const std::string& cmd) const {
    return cmd == "help" || cmd == "open" || cmd == "exit";
}

Command Controller::get_command(const Parser& input) {
    for (const Command& cmd : commands) {
        if (input.parsed_name() == cmd.name()) {
            return cmd;
        }
    }
    
    throw CommandNotFoundException{"Command not found"};
}

void Controller::run() {
    running_ = true;
    
    Parser input;
    while (running_) {
        out_ << "> ";
        in_ >> input;
        
        if (!path_.has_filename() && !basic(input.parsed_name())) {
            err_ << "First open a file\n"; continue;
        }
        
        try {
            get_command(input)(*this, input.parsed_args());
        }
        catch (const CustomException& e) {
            err_ << e.what() << "\n";
        }
        catch (const std::invalid_argument& e) {
            err_ << "Invalid ID\n";
        }
        catch (const std::out_of_range& e) {
            err_ << "ID not found\n";
        }
    }
}

void Controller::stop() {
    running_ = false;
}

void Controller::register_command(const std::string& name, const std::string& args, const std::string& usage, ulong args_count, const Function& f) {
    commands.emplace_back(name, args, usage, args_count, f);
}

bool Controller::init = Controller::init_commands();

bool Controller::init_commands() {
    register_command("list", "", "list all ids", 0,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            for (const Automaton& x : ctrl.automata_) {
                ctrl.out_ << x.id() << " ";
            }
        
            ctrl.out_ << std::endl;
        }
    );
    
    register_command("print", "<id>", "print automaton", 1,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            ctrl.out_ << ctrl.automata_.at(std::stoull(args[0]));
        }
    );

    register_command("empty", "<id>", "check if automaton's language is empty", 1,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            ctrl.out_ << std::boolalpha << ctrl.automata_.at(std::stoull(args[0])).empty() << "\n";
        }
    );

    register_command("deterministic", "<id>", "check if automaton is deterministic", 1,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            ctrl.out_ << std::boolalpha << ctrl.automata_.at(std::stoull(args[0])).deterministic() << "\n";
    });

    register_command("recognise", "<id> <word>", "check if automaton recognises word", 2,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            ctrl.out_ << std::boolalpha << ctrl.automata_.at(std::stoull(args[0])).recognises(args[1]) << "\n";
        }
    );
    
    register_command("reg", "<regex>", "create automaton from regular expression", 1,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            Automaton automaton{parse<Automaton>(args[0])->evaluate()};
            automaton.set_id(ctrl.automata_.size());

            ctrl.automata_.push_back(std::move(automaton));
                
            ctrl.out_ << "Automaton created. ID: " << automaton.id() << "\n";
        }
    );

    register_command("union", "<id1> <id2>", "find the union of two automata", 2,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            Automaton automaton{
                ctrl.automata_.at(std::stoull(args[0]))
                +
                ctrl.automata_.at(std::stoull(args[1]))
            };
            automaton.set_id(ctrl.automata_.size());

            ctrl.automata_.push_back(std::move(automaton));
                
            ctrl.out_ << "Automaton created. ID: " << automaton.id() << "\n";
        }
    );

    register_command("concat", "<id1> <id2>", "find the concatenation of two automata", 2,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            Automaton automaton{
                ctrl.automata_.at(std::stoull(args[0]))
                *
                ctrl.automata_.at(std::stoull(args[1]))
            };
            automaton.set_id(ctrl.automata_.size());

            ctrl.automata_.push_back(std::move(automaton));
                
            ctrl.out_ << "Automaton created. ID: " << automaton.id() << "\n";
        }
    );

    register_command("kleene", "<id>", "find the kleene closure of automaton", 1,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            Automaton automaton{*ctrl.automata_.at(std::stoull(args[0]))};
            automaton.set_id(ctrl.automata_.size());
                
            ctrl.automata_.push_back(std::move(automaton));

            ctrl.out_ << "Automaton created. ID: " << automaton.id() << "\n";
        }
    );
    
    register_command("convert", "<id>", "convert NFA to DFA", 1,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            Automaton automaton{ctrl.automata_.at(std::stoull(args[0]))};
            automaton.convert();
            automaton.set_id(ctrl.automata_.size());
        
            ctrl.automata_.push_back(std::move(automaton));
        
            ctrl.out_ << "Automaton created. ID: " << automaton.id() << "\n";
        }
    );

    register_command("open", "<filename>", "open file", 1,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            if (!ctrl.path_.empty()) {
                ctrl.out_ << "Another file is already opened\n"; return;
            }
        
            std::ifstream file{ctrl.path_ = args[0]};

            if (!file) {
                ctrl.err_ << "New file created\n"; return;
            }
            
            ctrl.out_ << "File opened successfully\n";
            
            
            ulong count{0};
            if (!std::filesystem::is_empty(ctrl.path_)) {
                Automaton temp;

                while (file >> temp) {
                    temp.set_id(ctrl.automata_.size());
                    ctrl.automata_.push_back(temp);
                    count++;
                }
            }
            
            ctrl.out_ << count << (count == 1 ? " automaton" : " automata") << " loaded\n";
        }
    );

    register_command("save", "", "save data to current file", 0,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            std::ofstream file{ctrl.path_};

            if (!file) {
                ctrl.err_ << "File could not be opened\n"; return;
            }

            for (const Automaton& a : ctrl.automata_) {
                file << a << "\n";
            }

            ctrl.out_ << "File successfully saved\n";
        }
    );

    register_command("saveas", "<filename>", "save data to file", 1,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            std::ofstream file{args[0]};

            if (!file) {
                ctrl.err_ << "File could not be opened\n"; return;
            }

            for (const Automaton& x : ctrl.automata_) {
                file << x << "\n";
            }

            ctrl.out_ << "File successfully saved\n";
        }
    );
    
    register_command("saveone", "<id> <filename>", "save automaton to file", 2,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            std::ofstream file{args[1]};

            if (!file) {
                ctrl.err_ << "File could not be opened\n"; return;
            }

            file << ctrl.automata_.at(std::stoull(args[0]));

            ctrl.out_ << "Atomaton " << args[0] << " successfully saved\n";
        }
    );

    register_command("close", "", "close current file", 0,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            ctrl.automata_.clear();
            ctrl.path_.clear();

            ctrl.out_ << "File closed successfully\n";
        }
    );

    register_command("help", "", "display this list", 0,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            ctrl.out_ << "Commands available:";

            for (const Command& x : commands) {
                ctrl.out_ << "\n\n  " << std::left << std::setw(27) << x;
            }

            ctrl.out_ << std::endl;
        }
    );

    register_command("exit", "", "exit the program", 0,
        [](Controller& ctrl, const std::vector<std::string>& args) {
            ctrl.out_ << "Exiting the program..." << std::endl;
        
            ctrl.stop();
        }
    );
    
    return true;
}
