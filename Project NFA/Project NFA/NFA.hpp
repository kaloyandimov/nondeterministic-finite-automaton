//
//  NFA.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef NFA_hpp
#define NFA_hpp

#include <set>
#include "State.hpp"

class NFA {
 public:
    NFA(const std::set<State>& states, const std::set<State>& final_states, const State& initial_state);
    
    const std::set<State>& get_states() const;
    const std::set<State>& get_final_states() const;
    size_t get_id() const;
    
    bool operator==(int num) const;
    
 private:
    std::set<State> states_;
    std::set<State> final_states_;
    State initial_state_;
    size_t id_;
    
    static size_t last_id_;
};

#endif /* NFA_hpp */
