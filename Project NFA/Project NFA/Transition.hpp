//
//  Transition.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef Transition_hpp
#define Transition_hpp

#include <memory>

class State;

class Transition {
 public:
    Transition() = default;
    Transition(std::shared_ptr<State> other, char symbol);
    
    std::shared_ptr<State> get_other() const;
    char get_symbol() const;
    
    void set_other(std::shared_ptr<State>);
    void set_symbol(char);
    
    bool operator<(const Transition&) const;
    
 private:
    std::shared_ptr<State> other_;
    char symbol_{'\0'};
};


#endif /* Transition_hpp */
