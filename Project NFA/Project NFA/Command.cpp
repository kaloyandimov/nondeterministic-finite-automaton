//
//  Command.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 30.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "Command.hpp"

void
Command::parse(const std::string& input) {
    std::istringstream ss{input};
    
    ss >> name_;
    
    std::copy(std::istream_iterator<std::string>{ss},
              std::istream_iterator<std::string>{},
              std::back_inserter(arguments_));
}

Command::Command(const std::string& input) {
    parse(input);
}

const std::string&
Command::name() const {
    return name_;
}

const Arguments&
Command::arguments() const {
    return arguments_;
}

Command::operator bool() const {
    return !name_.empty();
}

std::istream&
operator>>(std::istream& in, Command& command) {
    std::string input;
    std::getline(in, input);
    
    command.name_.clear();
    command.arguments_.clear();
    command.parse(input);
    
    return in;
}
