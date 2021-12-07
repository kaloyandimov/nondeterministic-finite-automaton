//
//  UnbalancedBracketsException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 6.12.21.
//

#ifndef UnbalancedBracketsException_hpp
#define UnbalancedBracketsException_hpp

#include <stdexcept>

class UnbalancedBracketsException: std::logic_error {
 public:
    UnbalancedBracketsException(const std::string& what_arg);
    
    char const* what() const noexcept override;
};

#endif /* UnbalancedBracketsException_hpp */
