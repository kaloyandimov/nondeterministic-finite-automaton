//
//  Parser.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <stdexcept>
#include <iterator>
#include <sstream>
#include <istream>
#include <string>
#include <vector>
#include <memory>

#include "Singleton.hpp"
#include "Union.hpp"
#include "Concatenation.hpp"
#include "KleeneClosure.hpp"

bool is_in_alphabet(char);
bool is_operator(char);
bool is_bracket(char);
bool is_metasymbol(char);
bool is_opening_token(char);
bool is_closing_token(char);

int priority(char);

bool only_metasymbols(const std::string&);
bool is_balanced(const std::string&);
bool is_correct(const std::string&, int = 0, bool = false);
bool is_valid(const std::string&);

std::string expand(const std::string&);
std::string shunting_yard(const std::string&);
std::unique_ptr<Expression> create(const std::string&);
std::unique_ptr<Expression> parse(const std::string&);

class Parser {
 public:
    explicit Parser(const std::string& = {});
    
    const std::string& parsed_name() const;
    const std::vector<std::string>& parsed_args() const;
    
    operator bool() const;
    
    friend std::istream& operator>>(std::istream&, Parser&);

 private:
    std::string name_;
    std::vector<std::string> args_;
    
    void parse(const std::string&);
};

#endif /* Parser_hpp */
