#include "controller/controller.hpp"

#include <exception>
#include <filesystem>
#include <fstream>

std::vector<Command> Controller::commands;

Controller::Controller(FileManager<Automaton, AutomatonSerializer> fm, std::istream& in, std::ostream& out, std::ostream& err) : fm_{fm}, in_{in}, out_{out}, err_{err}, running_{false} {}

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

        if (!in_) {
            out_ << std::endl;
            break;
        }

        if (!fm_.is_open() && !basic(input.parsed_name())) {
            err_ << "First open a file\n";
            continue;
        }

        try {
            get_command(input)(*this, input.parsed_args());
        } catch (const CustomException& e) {
            err_ << e.what() << "\n";
        } catch (const std::invalid_argument& e) {
            err_ << "Invalid ID\n";
        } catch (const std::out_of_range& e) {
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
                     });

    register_command("print", "<id>", "print automaton", 1,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         ctrl.automata_.at(std::stoull(args[0])).print(ctrl.out_);
                     });

    register_command("empty", "<id>", "check if automaton's language is empty", 1,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         ctrl.out_ << std::boolalpha << ctrl.automata_.at(std::stoull(args[0])).empty() << "\n";
                     });

    register_command("deterministic", "<id>", "check if automaton is deterministic", 1,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         ctrl.out_ << std::boolalpha << ctrl.automata_.at(std::stoull(args[0])).deterministic() << "\n";
                     });

    register_command("recognise", "<id> <word>", "check if automaton recognises word", 2,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         ctrl.out_ << std::boolalpha << ctrl.automata_.at(std::stoull(args[0])).recognises(args[1]) << "\n";
                     });

    register_command("reg", "<regex>", "create automaton from regular expression", 1,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         Automaton automaton{parse<Automaton>(args[0])->evaluate()};
                         automaton.set_id(ctrl.automata_.size());

                         ctrl.automata_.push_back(std::move(automaton));

                         ctrl.out_ << "Automaton created. ID: " << automaton.id() << "\n";
                     });

    register_command("union", "<id1> <id2>", "find the union of two automata", 2,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         Automaton automaton{
                             ctrl.automata_.at(std::stoull(args[0])) +
                             ctrl.automata_.at(std::stoull(args[1]))};
                         automaton.set_id(ctrl.automata_.size());

                         ctrl.automata_.push_back(std::move(automaton));

                         ctrl.out_ << "Automaton created. ID: " << automaton.id() << "\n";
                     });

    register_command("concat", "<id1> <id2>", "find the concatenation of two automata", 2,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         Automaton automaton{
                             ctrl.automata_.at(std::stoull(args[0])) *
                             ctrl.automata_.at(std::stoull(args[1]))};
                         automaton.set_id(ctrl.automata_.size());

                         ctrl.automata_.push_back(std::move(automaton));

                         ctrl.out_ << "Automaton created. ID: " << automaton.id() << "\n";
                     });

    register_command("kleene", "<id>", "find the kleene closure of automaton", 1,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         Automaton automaton{*ctrl.automata_.at(std::stoull(args[0]))};
                         automaton.set_id(ctrl.automata_.size());

                         ctrl.automata_.push_back(std::move(automaton));

                         ctrl.out_ << "Automaton created. ID: " << automaton.id() << "\n";
                     });

    register_command("convert", "<id>", "convert NFA to DFA", 1,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         Automaton automaton{ctrl.automata_.at(std::stoull(args[0]))};
                         automaton.convert();
                         automaton.set_id(ctrl.automata_.size());

                         ctrl.automata_.push_back(std::move(automaton));

                         ctrl.out_ << "Automaton created. ID: " << automaton.id() << "\n";
                     });

    register_command("open", "<filename>", "open file", 1,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         if (ctrl.fm_.is_open()) {
                             ctrl.out_ << "Another file is already opened\n";
                             return;
                         }

                         ctrl.fm_.open(args[0]);

                         if (!ctrl.fm_.file_exists()) {
                             ctrl.out_ << "New file created\n";
                             return;
                         }

                         ctrl.out_ << "File opened successfully\n";
                         ctrl.automata_ = ctrl.fm_.load();
                         ctrl.out_ << ctrl.automata_.size() << (ctrl.automata_.size() == 1 ? " automaton" : " automata") << " loaded\n";
                     });

    register_command("save", "", "save data to current file", 0,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         ctrl.fm_.save(ctrl.automata_);

                         ctrl.out_ << "File successfully saved\n";
                     });

    register_command("saveas", "<filename>", "save data to file", 1,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         ctrl.fm_.save_as(ctrl.automata_, args[0]);

                         ctrl.out_ << "File successfully saved\n";
                     });

    register_command("saveone", "<id> <filename>", "save automaton to file", 2,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         ctrl.fm_.save_as(ctrl.automata_.at(std::stoull(args[0])), args[1]);

                         ctrl.out_ << "Automaton " << args[0] << " successfully saved\n";
                     });

    register_command("close", "", "close current file", 0,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         ctrl.fm_.close();

                         ctrl.out_ << "File closed successfully\n";
                     });

    register_command("help", "", "display this list", 0,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         ctrl.out_ << "Commands available:";

                         for (const Command& x : commands) {
                             ctrl.out_ << "\n\n  " << std::left << std::setw(27) << x;
                         }

                         ctrl.out_ << std::endl;
                     });

    register_command("exit", "", "exit the program", 0,
                     [](Controller& ctrl, const std::vector<std::string>& args) {
                         ctrl.out_ << "Exiting the program..." << std::endl;

                         ctrl.stop();
                     });

    return true;
}
