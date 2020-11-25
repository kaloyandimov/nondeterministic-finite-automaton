//
//  RegExpr.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef RegExpr_hpp
#define RegExpr_hpp

#include <ostream>
#include <string>
#include <memory>

#include "Functions.hpp"
#include "NFA.hpp"

struct RegExpr {
    RegExpr() = default;
    RegExpr(const RegExpr&) = delete;
    RegExpr& operator=(const RegExpr&) = delete;
    virtual ~RegExpr() = default;
    
    virtual NFA evaluate() const = 0;
    virtual std::string print() const = 0;
    virtual std::unique_ptr<RegExpr> clone() const = 0;
    
    friend std::ostream& operator<<(std::ostream&, const RegExpr&);
};

#endif /* RegExpr_hpp */
