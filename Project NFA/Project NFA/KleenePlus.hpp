//
//  KleenePlus.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef KleenePlus_hpp
#define KleenePlus_hpp

#include <string>
#include <memory>

#include "Functions.hpp"
#include "RegExpr.hpp"
#include "NFA.hpp"

class KleenePlus: public RegExpr {
 public:
    KleenePlus(const RegExpr&);
    
    NFA evaluate() const override;
    std::string print() const override;
    std::unique_ptr<RegExpr> clone() const override;
    
 private:
    std::unique_ptr<RegExpr> expr;
};

#endif /* KleenePlus_hpp */
