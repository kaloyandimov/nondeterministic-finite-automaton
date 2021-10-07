//
//  Concatenation.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Concatenation_hpp
#define Concatenation_hpp

#include "Expression.hpp"

class Concatenation: public Expression {
 public:
    Concatenation(const Expression&, const Expression&);
    
    std::string print() const override;
    Automaton evaluate() const override;
    std::unique_ptr<Expression> clone() const override;
    
 private:
    std::unique_ptr<Expression> lhs_;
    std::unique_ptr<Expression> rhs_;
};

#endif /* Concatenation_hpp */
