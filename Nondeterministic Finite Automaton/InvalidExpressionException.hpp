//
//  InvalidExpressionException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 7.12.21.
//

#ifndef InvalidExpressionException_hpp
#define InvalidExpressionException_hpp

#include <stdexcept>

class InvalidExpressionException: std::logic_error {
 public:
    InvalidExpressionException(const std::string& what_arg);
    
    char const* what() const noexcept override;
};

#endif /* InvalidExpressionException_hpp */
