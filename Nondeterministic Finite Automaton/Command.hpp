//
//  Command.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Command_hpp
#define Command_hpp

#include <functional>
#include <optional>
#include <ostream>
#include <iomanip>
#include <string>
#include <vector>

class Controller;

using ulong = unsigned long;
using Function = std::function<void(Controller&, std::vector<std::string>)>;

class Command {
 public:
    Command() = default;
    Command(const std::string&, const std::string&, const std::string&, ulong, const Function&);
   
    const std::string& name() const;
    const std::string& args() const;
    const std::string& usage() const;
    ulong arg_count() const;
    
    bool valid() const;
    std::string title() const;
    std::string execute(Controller&, const std::vector<std::string>&) const;
    
    operator bool() const;
    std::string operator()(Controller&, const std::vector<std::string>&) const;
    
    friend std::ostream& operator<<(std::ostream&, const Command&);
   
 private:
    std::string name_;
    std::string args_;
    std::string usage_;
    ulong arg_count_;
    Function function_;
};
#endif /* Command_hpp */
