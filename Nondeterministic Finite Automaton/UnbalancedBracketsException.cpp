//
//  UnbalancedBracketsException.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 6.12.21.
//

#include "UnbalancedBracketsException.hpp"

UnbalancedBracketsException::UnbalancedBracketsException(const std::string& what_arg):
    std::logic_error{what_arg} {}

char const* UnbalancedBracketsException::what() const noexcept {
    return std::logic_error::what();
}
