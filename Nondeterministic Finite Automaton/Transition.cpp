//
//  Transition.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#include "Transition.hpp"

Transition::Transition(char symbol, ID endpoint):
    symbol_{symbol}, endpoint_{endpoint} {}

char Transition::symbol() const {
    return symbol_;
}

ID Transition::endpoint() const {
    return endpoint_;
}

void Transition::set_symbol(char symbol) {
    symbol_ = symbol;
}

void Transition::set_endpoint(ID endpoint) {
    endpoint_ = endpoint;
}

bool Transition::epsilon() const {
    return symbol_ == 'E';
}

bool Transition::operator<(const Transition& other) const {
    auto cmp = endpoint_ <=> other.endpoint_;
    
    return (cmp != 0 ? cmp : symbol_ <=> other.symbol_) < 0;
}

std::ostream& operator<<(std::ostream& out, const Transition& transition) {
    return out << transition.symbol_ << " " << transition.endpoint_;
}
