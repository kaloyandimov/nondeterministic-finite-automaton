//
//  NFA.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "NFA.hpp"

size_t NFA::last_id_{0};

NFA::NFA(const std::vector<State>& all_states,
         const std::vector<State>& final_states,
         const State& initial_state):
    all_states_{all_states},
    final_states_{final_states},
    initial_state_{initial_state},
    id_{last_id_++} {}

const std::vector<State>& NFA::get_all_states() const {
    return all_states_;
}

const std::vector<State>& NFA::get_final_states() const {
    return final_states_;
}

State NFA::get_initial_state() const {
    return initial_state_;
}

size_t NFA::get_id() const {
    return id_;
}

bool NFA::operator==(size_t id) const {
    return id_ == id;
}

std::ostream& operator<<(std::ostream& out, const NFA& automaton) {
    out << "Automaton " << automaton.get_id() << '\n';
    
    for (const State& state : automaton.get_all_states()) {
        out << "State " << state.get_id();
        
        out << (std::find(automaton.final_states_.begin(),
                          automaton.final_states_.end(),
                          state) != automaton.final_states_.end() ?
                          " (final)\n" : " (not final)\n");
        
        out << state << '\n';
    }
    
    return out;
}
