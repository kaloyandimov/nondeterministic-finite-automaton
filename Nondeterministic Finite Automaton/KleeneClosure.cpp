//
//  KleeneClosure.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#include "KleeneClosure.hpp"

KleeneClosure::KleeneClosure(const Expression& expr):
    expr_{expr.clone()} {}

std::string KleeneClosure::print() const {
    return "(" + expr_->print() + ")*";
}

Automaton KleeneClosure::evaluate() const {
    return *expr_->evaluate();
}

std::unique_ptr<Expression> KleeneClosure::clone() const {
    return std::make_unique<KleeneClosure>(*expr_);
}
