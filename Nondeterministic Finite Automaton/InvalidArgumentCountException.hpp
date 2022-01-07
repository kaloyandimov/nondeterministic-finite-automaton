//
//  InvalidArgumentCountException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 2.12.21.
//

#ifndef InvalidArgumentCountException_hpp
#define InvalidArgumentCountException_hpp

#include "CustomException.hpp"

class InvalidArgumentCountException: CustomException {
 public:
    InvalidArgumentCountException(const std::string& what_arg);
};

#endif /* InvalidArgumentCountException_hpp */
