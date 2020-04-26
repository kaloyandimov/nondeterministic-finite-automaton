//
//  State.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "State.hpp"

int State::s_last_id = 0;

State::State(bool is_final, const std::set<Transition>& transitions):
    transitions_{transitions},
    is_final_{is_final},
    id_{s_last_id++} {}

std::set<Transition> State::get_transitions() const {
    return transitions_;
}

int State::get_id() const {
    return id_;
}

bool State::get_is_final() const {
    return is_final_;
}

void State::set_transitions(const std::set<Transition>& transitions) {
    transitions_ = transitions;
}

void State::set_id(int id) {
    id_ = id;
}
void State::set_is_final(bool is_final) {
    is_final_ = is_final;
}
