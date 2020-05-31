//
//  State.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "State.hpp"

size_t
State::last_id_{0};

void
State::reset_last_id() {
    last_id_ = 0;
}

State::State(bool is_final):
    final_{is_final},
    id_{last_id_++} {
        std::sort(transitions_.begin(),
                  transitions_.end());
}

const std::vector<Transition>&
State::transitions() const {
    return transitions_;
}

bool
State::is_final() const {
    return final_;
}

size_t
State::id() const {
    return id_;
}

void
State::set_finality(bool is_final) {
    final_ = is_final;
}

void
State::rename(size_t number) {
    id_ += number;
    
    std::for_each(transitions_.begin(),
                  transitions_.end(),
                  [number](Transition& transition) {
        transition.second += number;
    });
}

void
State::add_transition(const Transition& transition) {
    transitions_.emplace(
        std::upper_bound(transitions_.begin(),
                         transitions_.end(),
                         transition),
        transition);
}

void
State::add_transition(char symbol, const State& state) {
    add_transition({symbol, state.id_});
}

void
State::add_epsilon_transition(const State& state) {
    add_transition({'E', state.id_});
}

bool
State::operator==(const State& other) const {
    return id_ == other.id_;
}

bool
State::operator<(const State& other) const {
    return id_ < other.id_;
}

bool
compare_by_id(const Transition& lhs, const Transition& rhs) {
    return lhs.second < rhs.second ||
        (!(lhs.second < rhs.second) && lhs.first < rhs.first);
}

std::ostream&
operator<<(std::ostream& out, const State& state) {
     std::vector<Transition> transitions{state.transitions_};
       std::sort(transitions.begin(),
                 transitions.end(),
                 compare_by_id);
       
       out << state.id_ << (state.is_final() ? "!": "") << '\n';
       
       std::for_each(transitions.begin(),
                     transitions.end(),
                     [&](const Transition& transition) {
           
           out << "-"
               << transition.first
               << "-> "
               << transition.second
               << "\n";
       });
    
    return out;
}
