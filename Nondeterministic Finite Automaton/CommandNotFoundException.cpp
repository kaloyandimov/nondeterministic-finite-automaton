//
//  CommandNotFoundException.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 2.12.21.
//

#include "CommandNotFoundException.hpp"

CommandNotFoundException::CommandNotFoundException(const std::string& what_arg):
    CustomException{what_arg} {}
