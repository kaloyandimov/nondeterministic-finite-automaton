//
//  Concatenation.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "Concatenation.hpp"

Concatenation::Concatenation(const RegExpr& _lhs, const RegExpr& _rhs):
    lhs{_lhs.clone()},
    rhs{_rhs.clone()} {}

NFA Concatenation::evaluate() const {
    return do_concatenation(lhs->evaluate(), rhs->evaluate());
}

std::string Concatenation::print() const {
    return "(" + lhs->print() + "." + rhs->print() + ")";
}

std::unique_ptr<RegExpr> Concatenation::clone() const {
    return std::make_unique<Concatenation>(*lhs, *rhs);
}
