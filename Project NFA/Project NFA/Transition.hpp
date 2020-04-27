//
//  Transition.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef Transition_hpp
#define Transition_hpp

#include "State.hpp"

class Transition {
public:
    Transition() = default;
    Transition(State lhs, State rhs, char symbol);
    
    void set_lhs(const State& lhs);
    void set_rhs(const State& rhs);
    void set_symbol(char symbol);
    
    State get_lhs() const;
    State get_rhs() const;
    char get_symbol() const;
    
    bool operator<(const Transition&) const;
    
private:
    State lhs_;
    State rhs_;
    char symbol_{'\0'};
};

#endif /* Transition_hpp */
