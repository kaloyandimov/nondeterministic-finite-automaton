//
//  KleenePlus.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "KleenePlus.hpp"

KleenePlus::KleenePlus(const RegExpr& _expr):
    expr{_expr.clone()} {}

NFA KleenePlus::evaluate() const {
    return do_kleene_plus(expr->evaluate());
}

std::string KleenePlus::print() const {
    return '(' + expr->print() + ')' + '*';
}

std::unique_ptr<RegExpr> KleenePlus::clone() const {
    return std::make_unique<KleenePlus>(*expr);
}
