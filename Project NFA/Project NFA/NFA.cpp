//
//  NFA.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "NFA.hpp"

int NFA::last_id_ = 0;

NFA::NFA(const std::set<State>& states, const std::set<Transition>& transitions, const State& initial_state):
    states_{states},
    transitions_{transitions},
    initial_state_{initial_state},
    id_{last_id_++} {}

void NFA::set_states(const std::set<State>& states) {
    states_ = states;
}

void NFA::set_transitions(const std::set<Transition>& transitions) {
    transitions_ = transitions;
}

void NFA::set_id(int id) {
    id_ = id;
}

std::set<State> NFA::get_states() const {
    return states_;
}

std::set<Transition> NFA::get_transitions() const {
    return transitions_;
}

int NFA::get_id() const {
    return id_;
}

std::ostream& operator<<(std::ostream& out, const NFA& nfa) {
    std::cout << "States:\n";
    
    for (auto state : nfa.states_) {
        std::cout << " " << state.get_id() << ": "
                  << (state.get_is_final() ? "Final" : "Not final") << std::endl;
    }
    
    std::cout << "\nTransitions:\n";
    
    for (auto transition : nfa.transitions_) {
        std::cout <<  " "   << transition.get_lhs().get_id();
        std::cout << " -> " << transition.get_rhs().get_id();
        std::cout <<  ": "  << transition.get_symbol() << std::endl;
    }

    
    return out;
}
