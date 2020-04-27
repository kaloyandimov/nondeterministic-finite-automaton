//
//  Transition.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "Transition.hpp"

Transition::Transition(State lhs, State rhs, char symbol):
    lhs_{lhs},
    rhs_{rhs},
    symbol_{symbol} {}

void Transition::set_lhs(const State& lhs) {
    lhs_ = lhs;
}

void Transition::set_rhs(const State& rhs) {
    rhs_ = rhs;
}

void Transition::set_symbol(char symbol) {
    symbol_ = symbol;
}

State Transition::get_lhs() const {
    return lhs_;
}

State Transition::get_rhs() const {
    return rhs_;
}

char Transition::get_symbol() const {
    return symbol_;
}

bool Transition::operator<(const Transition& other) const {
    return lhs_ < other.lhs_ ||
        (!(other.lhs_ < lhs_) && rhs_ < other.rhs_) ||
        (!(other.lhs_ < lhs_) && !(other.rhs_ < rhs_) && symbol_ < other.symbol_);
}
