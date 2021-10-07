//
//  Concatenation.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#include "Concatenation.hpp"

Concatenation::Concatenation(const Expression& lhs, const Expression& rhs):
    lhs_{lhs.clone()}, rhs_{rhs.clone()} {}

std::string Concatenation::print() const {
    return "(" + lhs_->print() + "." + rhs_->print() + ")";
}

Automaton Concatenation::evaluate() const {
    return lhs_->evaluate() * rhs_->evaluate();
}

std::unique_ptr<Expression> Concatenation::clone() const {
    return std::make_unique<Concatenation>(*lhs_, *rhs_);
}
