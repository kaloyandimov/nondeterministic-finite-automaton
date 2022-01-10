//
//  Singleton.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#include "Singleton.hpp"

Singleton::Singleton(char symbol):
    symbol_{symbol} {}

std::string Singleton::print() const {
    return {symbol_};
}

Automaton Singleton::evaluate() const {
    return Automaton{symbol_};
}

std::unique_ptr<Expression> Singleton::clone() const {
    return std::make_unique<Singleton>(symbol_);
}
