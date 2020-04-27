//
//  State.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 26.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

class State {
 public:
    explicit State(bool is_final = false);
    
    void set_id(int id) ;
    void set_is_final(bool is_final);
    
    int get_id() const;
    bool get_is_final() const;
    
    bool operator<(const State& other) const;
    
 private:
    int id_{-1};
    bool is_final_{false};
    
    static int last_id_;
};

#endif /* State_hpp */
