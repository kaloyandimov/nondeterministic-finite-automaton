//
//  InvalidArgumentCountException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 2.12.21.
//

#ifndef InvalidArgumentCountException_hpp
#define InvalidArgumentCountException_hpp

#include <stdexcept>

class InvalidArgumentCountException: std::logic_error {
 public:
    InvalidArgumentCountException(const std::string& what_arg);
    
    char const* what() const noexcept override;
};

#endif /* InvalidArgumentCountException_hpp */
