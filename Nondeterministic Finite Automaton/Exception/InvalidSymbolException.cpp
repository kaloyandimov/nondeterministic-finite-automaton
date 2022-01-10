//
//  InvalidSymbolException.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 6.12.21.
//

#include "InvalidSymbolException.hpp"

InvalidSymbolException::InvalidSymbolException(const std::string& what_arg):
    CustomException{what_arg} {}
