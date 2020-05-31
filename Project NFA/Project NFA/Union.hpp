//
//  Union.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef Union_hpp
#define Union_hpp

#include "RegExpr.hpp"

class Union: public RegExpr {
 public:
    Union(const std::unique_ptr<RegExpr>&,
          const std::unique_ptr<RegExpr>&);
    
    NFA evaluate() const override;
    std::string print() const override;
    std::unique_ptr<RegExpr> clone() const override;
    
 private:
    std::unique_ptr<RegExpr> lhs_;
    std::unique_ptr<RegExpr> rhs_;
};

#endif /* Union_hpp */
