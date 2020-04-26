//
//  State.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

#include <iostream>
#include <set>
#include "Transition.hpp"

class State {
 public:
    explicit State(bool is_final = false, const std::set<Transition>& transitions = {});
    
    std::set<Transition> get_transitions() const;
    int get_id() const;
    bool get_is_final() const;
    
    void set_transitions(const std::set<Transition>&);
    void set_id(int);
    void set_is_final(bool);
    
 private:
    std::set<Transition> transitions_;
    int id_{-1};
    bool is_final_{false};
    
    static int s_last_id;
};

#endif /* State_hpp */
