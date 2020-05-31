//
//  KleenePlus.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef KleenePlus_hpp
#define KleenePlus_hpp

#include "RegExpr.hpp"

class KleenePlus: public RegExpr {
 public:
    KleenePlus(const std::unique_ptr<RegExpr>&);
    
    NFA evaluate() const override;
    std::string print() const override;
    std::unique_ptr<RegExpr> clone() const override;
    
 private:
    std::unique_ptr<RegExpr> expression_;
};

#endif /* KleenePlus_hpp */
