//
//  Command.hpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 30.04.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#ifndef Command_hpp
#define Command_hpp

#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>

using Arguments = std::vector<std::string>;

class Command {
 public:
    Command() = default;
    Command(const std::string& input);
    
    const std::string& name() const;
    const Arguments& arguments() const;
    
    operator bool() const;
    
    friend std::istream& operator>>(std::istream& in, Command&);
    
 private:
    std::string name_;
    Arguments arguments_;
    
    void parse(const std::string& input);
};

#endif /* Command_hpp */
