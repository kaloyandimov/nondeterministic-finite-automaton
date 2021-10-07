//
//  Automaton.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Automaton_hpp
#define Automaton_hpp

#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <string>
#include <vector>
#include <stack>

#include "Transition.hpp"
#include "State.hpp"

class Automaton {
 public:
    explicit Automaton(const std::vector<State>& = {}, ID initial_state = 0, ID id = 0);
    explicit Automaton(char, ID initial_state = 0, ID id = 0);

    std::vector<State> states() const;
    ID initial_state() const;
    ID id() const;

    void set_states(const std::vector<State>&);
    void set_initial_state(ID);
    void set_id(ID);
    
    bool empty() const;
    bool deterministic() const;
    bool recognises(const std::string&) const;
    
    ID max_state_id() const;
    ulong transition_count() const;
    
    void normalise();
    
    Automaton operator+(const Automaton&) const;
    Automaton operator*(const Automaton&) const;
    Automaton operator*() const;
    
    friend std::istream& operator>>(std::istream&, Automaton&);
    friend std::ostream& operator<<(std::ostream&, const Automaton&);
    
 private:
    std::vector<State> states_;
    ID initial_state_;
    ID id_;
    
    bool recognises_util(const std::string&, ID, ulong) const;
};

#endif /* Automaton_hpp */
