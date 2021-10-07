//
//  Union.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#include "Union.hpp"

Union::Union(const Expression& lhs, const Expression& rhs):
    lhs_{lhs.clone()}, rhs_{rhs.clone()} {}

std::string Union::print() const {
    return "(" + lhs_->print() + "+" + rhs_->print() + ")";
}

Automaton Union::evaluate() const {
    return lhs_->evaluate() + rhs_->evaluate();
}

std::unique_ptr<Expression> Union::clone() const {
    return std::make_unique<Union>(*lhs_, *rhs_);
}
