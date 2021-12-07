//
//  InvalidSymbolException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 6.12.21.
//

#ifndef InvalidSymbolException_hpp
#define InvalidSymbolException_hpp

#include <stdexcept>

class InvalidSymbolException: std::logic_error {
 public:
    InvalidSymbolException(const std::string& what_arg);

    char const* what() const noexcept override;
};

#endif /* InvalidSymbolException_hpp */
