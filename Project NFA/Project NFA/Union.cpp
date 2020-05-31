//
//  Union.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "Union.hpp"

Union::Union(const std::unique_ptr<RegExpr>& lhs,
             const std::unique_ptr<RegExpr>& rhs):
    lhs_{lhs->clone()},
    rhs_{rhs->clone()} {}

NFA
Union::evaluate() const {
    return union_(lhs_->evaluate(), rhs_->evaluate());
}

std::string
Union::print() const {
    return '(' + lhs_->print() + '+' + rhs_->print() + ')';;
}

std::unique_ptr<RegExpr>
Union::clone() const {
    return std::make_unique<Union>(lhs_, rhs_);
}
