//
//  UnbalancedBracketsException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 6.12.21.
//

#ifndef UnbalancedBracketsException_hpp
#define UnbalancedBracketsException_hpp

#include "CustomException.hpp"

class UnbalancedBracketsException: public CustomException {
 public:
    UnbalancedBracketsException(const std::string& what_arg);
};

#endif /* UnbalancedBracketsException_hpp */
