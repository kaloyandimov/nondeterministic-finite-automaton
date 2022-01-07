//
//  InvalidExpressionException.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 7.12.21.
//

#include "InvalidExpressionException.hpp"

InvalidExpressionException::InvalidExpressionException(const std::string& what_arg):
    CustomException{what_arg} {}
