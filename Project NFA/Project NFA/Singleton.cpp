//
//  Singleton.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "Singleton.hpp"

Singleton::Singleton(char symbol):
    symbol_{symbol} {}

NFA
Singleton::evaluate() const {
    return singleton_(symbol_);
}

std::string
Singleton::print() const {
    return {symbol_};
}

std::unique_ptr<RegExpr>
Singleton::clone() const {
    return std::make_unique<Singleton>(symbol_);
}
