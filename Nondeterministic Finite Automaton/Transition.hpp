//
//  Transition.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Transition_hpp
#define Transition_hpp

#include <ostream>
#include <compare>

using ID = unsigned long;

class Transition {
 public:
    Transition(char, ID);
    
    char symbol() const;
    ID endpoint() const;
    
    void set_symbol(char);
    void set_endpoint(ID);
    
    bool epsilon() const;
    
    bool operator<(const Transition&) const;
    bool operator==(const Transition&) const = default;
    
    friend std::ostream& operator<<(std::ostream&, const Transition&);
    
 private:
    char symbol_;
    ID endpoint_;
};

#endif /* Transition_hpp */
