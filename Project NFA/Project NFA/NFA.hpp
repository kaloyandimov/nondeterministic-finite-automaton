//
//  NFA.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef NFA_hpp
#define NFA_hpp

#include "State.hpp"

class NFA {
 public:
    NFA(const std::vector<State>& all_states,
        const std::vector<State>& final_states,
        const State& initial_state);
    
    const std::vector<State>& all_states() const;
    const std::vector<State>& final_states() const;
    const State& initial_state() const;
    size_t id() const;
    
    void set_id(size_t);
    
    bool operator==(size_t id) const;
    
    friend std::ostream& operator<<(std::ostream&, const NFA&);
    
 private:
    std::vector<State> all_states_;
    std::vector<State> final_states_;
    State initial_state_;
    size_t id_;
    
    static size_t last_id_;
};

#endif /* NFA_hpp */
