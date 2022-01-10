//
//  KleeneClosure.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef KleeneClosure_hpp
#define KleeneClosure_hpp

#include "Expression.hpp"

class KleeneClosure : public Expression {
 public:
    KleeneClosure(const Expression&);
   
    std::string print() const override;
    Automaton evaluate() const override;
    std::unique_ptr<Expression> clone() const override;
   
 private:
    std::unique_ptr<Expression> expr_;
};

#endif /* KleeneClosure_hpp */
