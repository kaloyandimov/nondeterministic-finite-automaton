//
//  Singleton.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef Singleton_hpp
#define Singleton_hpp

#include <string>
#include <memory>

#include "Functions.hpp"
#include "RegExpr.hpp"
#include "NFA.hpp"

class Singleton: public RegExpr {
 public:
    Singleton(char);
    
    NFA evaluate() const override;
    std::string print() const override;
    std::unique_ptr<RegExpr> clone() const override;
    
 private:
    char symbol;
};

#endif /* Singleton_hpp */
