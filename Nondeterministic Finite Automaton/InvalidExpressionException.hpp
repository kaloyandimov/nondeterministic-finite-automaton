//
//  InvalidExpressionException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 7.12.21.
//

#ifndef InvalidExpressionException_hpp
#define InvalidExpressionException_hpp

#include "CustomException.hpp"

class InvalidExpressionException: CustomException {
 public:
    InvalidExpressionException(const std::string& what_arg);
};

#endif /* InvalidExpressionException_hpp */
