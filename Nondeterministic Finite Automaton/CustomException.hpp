//
//  CustomException.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 7.01.22.
//

#ifndef CustomException_hpp
#define CustomException_hpp

#include <stdexcept>

class CustomException: public std::logic_error {
 public:
    CustomException(const std::string& what_arg);
    
    char const* what() const noexcept override;
};

#endif /* CustomException_hpp */
