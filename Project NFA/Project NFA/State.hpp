//
//  State.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>

using Transition = std::pair<char, size_t>;

class State {
 public:
    explicit State(bool is_final = false);
    
    const std::vector<Transition>& transitions() const;
    bool is_final() const;
    size_t id() const;
    
    void set_finality(bool);
    
    void rename(size_t);
    void add_transition(const Transition&);
    void add_transition(char, const State&);
    void add_epsilon_transition(const State&);

    static void reset_last_id();
    
    bool operator==(const State&) const;
    bool operator<(const State&) const;
    
    friend std::ostream& operator<<(std::ostream&, const State&);
    
 private:
    std::vector<Transition> transitions_;
    bool final_;
    size_t id_;
    
    static size_t last_id_;
};

#endif /* State_hpp */
