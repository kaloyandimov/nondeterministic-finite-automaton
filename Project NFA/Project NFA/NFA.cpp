//
//  NFA.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "NFA.hpp"

size_t
NFA::last_id_{0};

NFA::NFA(const std::vector<State>& all_states,
         const std::vector<State>& final_states,
         const State& initial_state):
    all_states_{all_states},
    final_states_{final_states},
    initial_state_{initial_state},
    id_{last_id_++} {
        std::sort(all_states_.begin(),
                  all_states_.end());
        std::sort(final_states_.begin(),
                  final_states_.end());
        
        State::reset_last_id();
    }

const std::vector<State>&
NFA::all_states() const {
    return all_states_;
}

const std::vector<State>&
NFA::final_states() const {
    return final_states_;
}

const State&
NFA::initial_state() const {
    return initial_state_;
}

size_t
NFA::id() const {
    return id_;
}

void
NFA::set_id(size_t id) {
    id_ = id;
}

bool
NFA::operator==(size_t id) const {
    return id_ == id;
}

std::ostream&
operator<<(std::ostream& out, const NFA& automaton) {
    std::for_each(automaton.all_states().begin(),
                  automaton.all_states().end(),
                  [&](const State& state) {
        out << state;
    });
    
    return out;
}
