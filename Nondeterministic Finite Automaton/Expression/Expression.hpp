//
//  Expression.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Expression_hpp
#define Expression_hpp

#include <ostream>
#include <string>
#include <memory>

#include "Automaton.hpp"

class Expression {
 public:
    Expression() = default;
    Expression(const Expression&) = delete;
    Expression& operator=(const Expression&) = delete;
    Expression(Expression&&) = default;
    Expression& operator=(Expression&&) = default;
    virtual ~Expression() = default;
    
    virtual std::string print() const = 0;
    virtual Automaton evaluate() const = 0;
    virtual std::unique_ptr<Expression> clone() const = 0;
    
    friend std::ostream& operator<<(std::ostream&, const Expression&);
};

#endif /* Expression_hpp */
