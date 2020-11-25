//
//  Union.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef Union_hpp
#define Union_hpp

#include <string>
#include <memory>

#include "Functions.hpp"
#include "RegExpr.hpp"
#include "NFA.hpp"

class Union: public RegExpr {
 public:
    Union(const RegExpr&, const RegExpr&);
    
    NFA evaluate() const override;
    std::string print() const override;
    std::unique_ptr<RegExpr> clone() const override;
    
 private:
    std::unique_ptr<RegExpr> lhs;
    std::unique_ptr<RegExpr> rhs;
};

#endif /* Union_hpp */
