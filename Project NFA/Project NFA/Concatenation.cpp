//
//  Concatenation.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "Concatenation.hpp"

Concatenation::Concatenation(const std::unique_ptr<RegExpr>& lhs,
                             const std::unique_ptr<RegExpr>& rhs):
    lhs_{lhs->clone()},
    rhs_{rhs->clone()} {}

NFA Concatenation::evaluate() const {
    return concatenation_(lhs_->evaluate(), rhs_->evaluate());
}

std::string Concatenation::print() const {
    return '(' + lhs_->print() + '.' + rhs_->print() + ')';
}

std::unique_ptr<RegExpr> Concatenation::clone() const {
    return std::make_unique<Concatenation>(lhs_, rhs_);
}
