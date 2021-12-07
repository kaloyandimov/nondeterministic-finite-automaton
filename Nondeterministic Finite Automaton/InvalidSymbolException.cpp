//
//  InvalidSymbolException.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 6.12.21.
//

#include "InvalidSymbolException.hpp"

InvalidSymbolException::InvalidSymbolException(const std::string& what_arg):
    std::logic_error{what_arg} {}

char const* InvalidSymbolException::what() const noexcept {
    return std::logic_error::what();
}
