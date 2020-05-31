//
//  Functions.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 16.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef Functions_hpp
#define Functions_hpp

#include <iterator>
#include "NFA.hpp"

NFA singleton_(char symbol);
NFA union_(const NFA&, const NFA&);
NFA concatenation_(const NFA&, const NFA&);
NFA kleene_plus_(const NFA&);

#endif /* Functions_hpp */
