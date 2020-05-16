//
//  State.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "State.hpp"

size_t State::last_id_{0};

State::State():
    id_{last_id_++} {
        std::sort(transitions_.begin(),
                  transitions_.end());
}

const std::vector<Transition>& State::get_transitions() const {
    return transitions_;
}

size_t State::get_id() const {
    return id_;
}

void State::add_epsilon_transition(State state) {
    add_transition({'E', state});
}

void State::add_transition(char symbol, State state) {
    add_transition({symbol, state});
}

void State::add_transition(const Transition& transition) {
    std::vector<std::pair<char, State>>::iterator position{
        std::upper_bound(transitions_.begin(),
                         transitions_.end(),
                         transition)
    };
    
    transitions_.emplace(position, transition);
}

bool State::operator<(const State& other) const {
    return id_ < other.id_;
}

bool State::operator==(const State& other) const {
    return id_ == other.id_;
}

bool compare(const Transition& lhs, const Transition& rhs) {
    return lhs.second < rhs.second ||
           (!(lhs.second < rhs.second) && lhs.first < rhs.first);
}

std::ostream& operator<<(std::ostream& out, const State& state) {
    std::vector<Transition> transitions{state.transitions_};
    std::sort(transitions.begin(), transitions.end(), compare);
    
    for (const Transition& transition : transitions) {
        out << state.id_ << " --> " << transition.second.id_
            << ": " << transition.first << '\n';
    }
    
    return out;
}
