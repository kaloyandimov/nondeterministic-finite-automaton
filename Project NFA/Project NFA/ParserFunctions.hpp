//
//  ParserFunctions.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef ParserFunctions_hpp
#define ParserFunctions_hpp

#include <stack>
#include "Singleton.hpp"
#include "Concatenation.hpp"
#include "Union.hpp"
#include "KleenePlus.hpp"

std::string shunting_yard(const std::string& infix);
std::unique_ptr<RegExpr> parse(const std::string& postfix);

#endif /* ParserFunctions_hpp */
