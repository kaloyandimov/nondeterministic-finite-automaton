//
//  InvalidSymbolException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 6.12.21.
//

#ifndef InvalidSymbolException_hpp
#define InvalidSymbolException_hpp

#include "CustomException.hpp"

class InvalidSymbolException: public CustomException {
 public:
    InvalidSymbolException(const std::string& what_arg);
};

#endif /* InvalidSymbolException_hpp */
