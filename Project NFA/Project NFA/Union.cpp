//
//  Union.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "Union.hpp"

Union::Union(const RegExpr& _lhs, const RegExpr& _rhs):
    lhs{_lhs.clone()},
    rhs{_rhs.clone()} {}

NFA Union::evaluate() const {
    return do_union(lhs->evaluate(), rhs->evaluate());
}

std::string Union::print() const {
    return "(" + lhs->print() + "+" + rhs->print() + ")";
}

std::unique_ptr<RegExpr> Union::clone() const {
    return std::make_unique<Union>(*lhs, *rhs);
}
