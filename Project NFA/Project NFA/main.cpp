//
//  main.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "NFA.hpp"

int main() {
//    --> @ --a-> * --b-> @
    
    State state0{true};
    State state1;
    State state2{true};
    
    Transition transition0{state0, state1, 'a'};
    Transition transition1{state1, state2, 'b'};
    
    std::set<State> states{state0, state1, state2};
    std::set<Transition> transitions{transition0, transition1};
    
    NFA nfa{states, transitions, state1};
    
    std::cout << nfa;
    
    return 0;
}
