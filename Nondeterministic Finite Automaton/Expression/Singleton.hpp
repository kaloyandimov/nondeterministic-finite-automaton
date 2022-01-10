//
//  Singleton.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Singleton_hpp
#define Singleton_hpp

#include "Expression.hpp"

class Singleton : public Expression {
 public:
    Singleton(char);
    
    std::string print() const override;
    Automaton evaluate() const override;
    std::unique_ptr<Expression> clone() const override;
    
 private:
    char symbol_;
};

#endif /* Singleton_hpp */
