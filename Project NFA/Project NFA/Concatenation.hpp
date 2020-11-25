//
//  Concatenation.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef Concatenation_hpp
#define Concatenation_hpp

#include <string>
#include <memory>

#include "Functions.hpp"
#include "RegExpr.hpp"
#include "NFA.hpp"

class Concatenation: public RegExpr {
 public:
    Concatenation(const RegExpr&, const RegExpr&);
    
    NFA evaluate() const override;
    std::string print() const override;
    std::unique_ptr<RegExpr> clone() const override;
    
 private:
    std::unique_ptr<RegExpr> lhs;
    std::unique_ptr<RegExpr> rhs;
};

#endif /* Concatenation_hpp */
