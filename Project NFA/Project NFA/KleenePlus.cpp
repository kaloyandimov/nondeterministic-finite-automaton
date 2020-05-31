//
//  KleenePlus.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "KleenePlus.hpp"

KleenePlus::KleenePlus(const std::unique_ptr<RegExpr>& expression):
    expression_{expression->clone()} {}

NFA KleenePlus::evaluate() const {
    return kleene_plus_(expression_->evaluate());
}

std::string KleenePlus::print() const {
    return '(' + expression_->print() + ')' + '*';
}

std::unique_ptr<RegExpr> KleenePlus::clone() const {
    return std::make_unique<KleenePlus>(expression_);
}
