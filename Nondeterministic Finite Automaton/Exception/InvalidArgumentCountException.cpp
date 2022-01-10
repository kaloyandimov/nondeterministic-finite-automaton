//
//  InvalidArgumentCountException.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 2.12.21.
//

#include "InvalidArgumentCountException.hpp"

InvalidArgumentCountException::InvalidArgumentCountException(const std::string& what_arg):
    CustomException{what_arg} {}
