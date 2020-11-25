//
//  RegExpr.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "RegExpr.hpp"

std::ostream& operator<<(std::ostream& out, const RegExpr& expr) {
    return out << expr.print();
}
