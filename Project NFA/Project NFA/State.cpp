//
//  State.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "State.hpp"

size_t State::last_id_{0};

State::State(const std::multimap<State, char>& transitions):
    transitions_{transitions},
    id_{last_id_++} {}

const std::multimap<State, char>& State::get_transitions() const {
    return transitions_;
}

size_t State::get_id() const {
    return id_;
}

void State::add_transition(const std::pair<State, char>& transition) {
    transitions_.emplace(transition);
}

bool State::operator<(const State& other) const {
    return id_ < other.id_;
}

std::ostream& operator<<(std::ostream& out, const State& state) {
    for (std::pair<State,char> transition : state.transitions_) {
        out << state.id_ << " --> " << transition.first.id_ << ": " << transition.second << '\n';
    }
    
    return out;
}
