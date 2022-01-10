//
//  State.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef State_hpp
#define State_hpp

#include <ostream>
#include <vector>
#include <limits>

#include "Transition.hpp"

using ulong = unsigned long;

class State {
 public:
    explicit State(bool = false, ID = 0);
    
    bool accepting() const;
    ID id() const;
    const std::vector<Transition>& transitions() const;

    void set_accepting(bool);
    void set_id(ID);
    void set_transitions(const std::vector<Transition>&);

    bool deterministic() const;
    
    ulong transition_count() const;
    
    void add_transition(const Transition&);
    void add_transition(char, ID);
    void add_epsilon_transition(ID);
    void add_to_ids(ulong);
    
    bool operator==(const State&) const;
    bool operator<(const State&) const;
    
    friend std::ostream& operator<<(std::ostream&, const State&);
    
 private:
    bool accepting_;
    ID id_;
    std::vector<Transition> transitions_;
};

#endif /* State_hpp */
