//
//  InvalidExpressionException.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 7.12.21.
//

#include "InvalidExpressionException.hpp"

InvalidExpressionException::InvalidExpressionException(const std::string& what_arg):
    std::logic_error{what_arg} {}

char const* InvalidExpressionException::what() const noexcept {
    return std::logic_error::what();
}
