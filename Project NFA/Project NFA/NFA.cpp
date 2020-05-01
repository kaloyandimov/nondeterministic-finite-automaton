//
//  NFA.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "NFA.hpp"

size_t NFA::last_id_{0};

NFA::NFA(const std::set<State>& states, const std::set<State>& final_states, const State& initial_state):
    states_{states},
    final_states_{final_states},
    initial_state_{initial_state},
    id_{last_id_++} {}

const std::set<State>& NFA::get_states() const {
    return states_;
}

const std::set<State>& NFA::get_final_states() const {
    return final_states_;
}

size_t NFA::get_id() const {
    return id_;
}

bool NFA::operator==(int num) const {
    return id_ == num;
}
