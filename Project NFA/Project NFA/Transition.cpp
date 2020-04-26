//
//  Transition.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "Transition.hpp"

Transition::Transition(std::shared_ptr<State> other, char symbol):
    symbol_{symbol}, other_{other} {}

char Transition::get_symbol() const {
    return symbol_;
}

std::shared_ptr<State> Transition::get_other() const {
    return other_;
}

void Transition::set_symbol(char symbol) {
    symbol_ = symbol;
}

void Transition::set_other(std::shared_ptr<State> other) {
    other_ = other;
}

bool Transition::operator<(const Transition& other) const {
    return symbol_ < other.symbol_;
}
