//
//  InvalidArgumentCountException.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 2.12.21.
//

#include "InvalidArgumentCountException.hpp"

InvalidArgumentCountException::InvalidArgumentCountException(const std::string& what_arg):
    std::logic_error{what_arg} {}

char const* InvalidArgumentCountException::what() const noexcept {
    return std::logic_error::what();
}
