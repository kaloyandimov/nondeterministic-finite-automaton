//
//  State.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

#include <map>
#include <utility>
#include <iostream>

class State {
 public:
    State(const std::multimap<State, char>& transitions = {});
    
    const std::multimap<State, char>& get_transitions() const;
    size_t get_id() const;
    
    void add_transition(const std::pair<State, char>& transition);
    
    bool operator<(const State& other) const;
    
    friend std::ostream& operator<<(std::ostream&, const State&);
    
 private:
    std::multimap<State, char> transitions_;
    size_t id_;
    
    static size_t last_id_;
};

#endif /* State_hpp */
