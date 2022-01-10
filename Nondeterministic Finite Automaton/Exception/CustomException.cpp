//
//  CustomException.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 7.01.22.
//

#include "CustomException.hpp"

CustomException::CustomException(const std::string& what_arg):
    std::logic_error{what_arg} {}

char const* CustomException::what() const noexcept {
    return std::logic_error::what();
}
