//
//  Expression.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#include "Expression.hpp"

std::ostream& operator<<(std::ostream& out, const Expression& expression) {
    return out << expression.print();
}
