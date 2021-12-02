//
//  CommandNotFoundException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 2.12.21.
//

#ifndef CommandNotFoundException_hpp
#define CommandNotFoundException_hpp

#include <stdexcept>

class CommandNotFoundException: std::logic_error {
 public:
    CommandNotFoundException(const std::string& what_arg);
    
    char const* what() const noexcept override;
};

#endif /* CommandNotFoundException_hpp */
