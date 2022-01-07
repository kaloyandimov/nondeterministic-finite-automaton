//
//  CommandNotFoundException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 2.12.21.
//

#ifndef CommandNotFoundException_hpp
#define CommandNotFoundException_hpp

#include "CustomException.hpp"

class CommandNotFoundException: CustomException {
 public:
    CommandNotFoundException(const std::string& what_arg);
};

#endif /* CommandNotFoundException_hpp */
