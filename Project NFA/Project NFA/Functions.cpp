//
//  Functions.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 16.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "Functions.hpp"

State
rename(State state, size_t number) {
    state.rename(number);
    
    return state;
}

std::vector<State>
rename(std::vector<State> states, size_t number) {
    std::for_each(states.begin(),
                  states.end(),
                  [&](State& state) {
        state.rename(number);
    });
    
    return states;
}

void change_finality(const State& state,
                     std::vector<State>& states,
                     bool is_final) {
    auto position{
        std::find(states.begin(),
                  states.end(),
                  state)
    };
    
    if (position != states.end()) {
        position->set_finality(is_final);
    }
}

void
remove_state(const State& state,
             std::vector<State>& states) {
    auto position{
        std::find(states.begin(),
                  states.end(),
                  state)
    };
    
    if (position != states.end()) {
        states.erase(position);
    }
}

std::vector<State>
merge_states(const std::vector<State>& lhs,
             const std::vector<State>& rhs) {
    std::vector<State> result{lhs};
    
    std::copy(rhs.begin(),
              rhs.end(),
              std::back_inserter(result));
    
    return result;
}



NFA
singleton_(char symbol) {
    State initial_state;
    State final_state{true};
    
    initial_state.add_transition(symbol, final_state);
    
    return NFA{{initial_state, final_state},
               {final_state},
               initial_state};
}

NFA
union_(const NFA& lhs, const NFA& rhs) {
    const size_t number{lhs.all_states().size() + 1};
    std::vector<State> all_states{
        merge_states(rename(lhs.all_states(), 1),
                     rename(rhs.all_states(), number))
    };
    std::vector<State> final_states{
        merge_states(rename(lhs.final_states(), 1),
                     rename(rhs.final_states(), number))
    };
    
    State new_initial;
    State lhs_old_initial{rename(lhs.initial_state(), 1)};
    State rhs_old_initial{rename(rhs.initial_state(), number)};
    
    change_finality(lhs_old_initial, all_states, false);
    change_finality(rhs_old_initial, all_states, false);
    
    remove_state(lhs_old_initial, final_states);
    remove_state(rhs_old_initial, final_states);
    
    new_initial.add_epsilon_transition(lhs_old_initial);
    new_initial.add_epsilon_transition(rhs_old_initial);
    
    all_states.emplace_back(new_initial);

    return NFA{all_states, final_states, new_initial};
}



NFA
concatenation_(const NFA& lhs, const NFA& rhs) {
    const size_t number{lhs.all_states().size()};
    std::vector<State> all_states{
        merge_states(lhs.all_states(),
                     rename(rhs.all_states(), number))
    };
    
    std::vector<State> lhs_final_states{lhs.final_states()};
    std::vector<State> rhs_final_states{
        rename(rhs.final_states(), number)
    };
    
    State rhs_initial_state{rename(rhs.initial_state(), number)};
    std::for_each(lhs_final_states.begin(),
                  lhs_final_states.end(),
                  [&](const State& final_state) {
        State& state{
            *std::find(all_states.begin(),
                       all_states.end(),
                       final_state)
        };
        
        state.set_finality(false);
        state.add_epsilon_transition(rhs_initial_state);
    });
    
    return NFA{all_states, rhs_final_states, lhs.initial_state()};
}

NFA
kleene_plus_(const NFA& automaton) {
    std::vector<State> all_states{rename(automaton.all_states(), 1)};
    State new_initial;
    State old_initial{rename(automaton.initial_state(), 1)};
    
    change_finality(old_initial, all_states, false);
    
    new_initial.add_epsilon_transition(old_initial);
    all_states.emplace_back(new_initial);
    
    std::vector<State> final_states;
    std::for_each(all_states.begin(),
                  all_states.end(),
                  [&](State& state) {
        if (state.is_final()) {
            state.add_epsilon_transition(old_initial);
            final_states.emplace_back(state);
        }
    });
    
    return NFA{all_states, final_states, new_initial};
}
