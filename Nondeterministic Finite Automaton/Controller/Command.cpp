//
//  Command.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#include "Command.hpp"

Command::Command(const std::string& name, const std::string& args, const std::string& usage, ulong arg_count, const Function& function):
    name_{name}, args_{args}, usage_{usage}, arg_count_{arg_count}, function_{function} {}

const std::string& Command::name() const {
    return name_;
}

const std::string& Command::args() const {
    return args_;
}

const std::string& Command::usage() const {
    return usage_;
}

ulong Command::arg_count() const {
    return arg_count_;
}

bool Command::valid() const {
    return !name_.empty();
}

std::string Command::title() const {
    return name_ + (arg_count_ == 0 ? "" : " " + args_);
}

void Command::execute(Controller& ctrl, const std::vector<std::string>& args) const {
    if (args.size() < arg_count_) {
        throw InvalidArgumentCountException{"Too few arguments"};
    }
    
    if (args.size() > arg_count_) {
        throw InvalidArgumentCountException{"Too many arguments"};
    }
    
    function_(ctrl, args);
}

Command::operator bool() const {
    return valid();
}

void Command::operator()(Controller& ctrl, const std::vector<std::string>& args) const {
    execute(ctrl, args);
}

std::ostream& operator<<(std::ostream& out, const Command& cmd) {
    return out << cmd.title() << cmd.usage();
}
