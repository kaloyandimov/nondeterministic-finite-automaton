//
//  CommandNotFoundException.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 2.12.21.
//

#include "CommandNotFoundException.hpp"

CommandNotFoundException::CommandNotFoundException(const std::string& what_arg):
    std::logic_error{what_arg} {}

char const* CommandNotFoundException::what() const noexcept {
    return std::logic_error::what();
}
