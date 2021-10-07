//
//  Union.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Union_hpp
#define Union_hpp

#include "Expression.hpp"

class Union : public Expression {
 public:
    Union(const Expression&, const Expression&);
    
    std::string print() const override;
    Automaton evaluate() const override;
    std::unique_ptr<Expression> clone() const override;
    
 private:
    std::unique_ptr<Expression> lhs_;
    std::unique_ptr<Expression> rhs_;
};

#endif /* Union_hpp */
