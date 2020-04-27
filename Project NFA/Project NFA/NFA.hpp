//
//  NFA.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef NFA_hpp
#define NFA_hpp

#include <iostream>
#include <set>
#include "Transition.hpp"

class NFA{
 public:
    NFA() = default;
    NFA(const std::set<State>& states, const std::set<Transition>& transitions, const State& initial_state);
    
    void set_states(const std::set<State>& states);
    void set_transitions(const std::set<Transition>& transitions);
    void set_id(int id);
    
    std::set<State> get_states() const;
    std::set<Transition> get_transitions() const;
    int get_id() const;
    
    friend std::ostream& operator<<(std::ostream& out, const NFA& nfa);
    
 private:
    std::set<State> states_;
    std::set<Transition> transitions_;
    State initial_state_;
    int id_{-1};
    
    static int last_id_;
};

#endif /* NFA_hpp */
