//
//  Parser.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <istream>
#include <iterator>
#include <memory>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "Atom.hpp"
#include "Automaton.hpp"
#include "Concatenation.hpp"
#include "InvalidExpressionException.hpp"
#include "InvalidSymbolException.hpp"
#include "KleeneStar.hpp"
#include "UnbalancedBracketsException.hpp"
#include "Union.hpp"

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

template<typename T>
std::unique_ptr<Expression<T>> create(const std::string&);

template<typename T>
std::unique_ptr<Expression<T>> parse(const std::string&);

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

template<typename T>
std::unique_ptr<Expression<T>> create(const std::string& postfix) {
    std::stack<std::unique_ptr<Expression<T>>> expressions;
    
    for (char c : postfix) {
        if (is_in_alphabet(c)) {
            expressions.push(std::make_unique<Atom<T>>(c));
        }
        else if (c == '*') {
            std::unique_ptr<Expression<T>> expr{std::move(expressions.top())};
            expressions.pop();
            
            expressions.push(std::make_unique<KleeneStar<T>>(*expr));
        }
        else if (c == '.') {
            std::unique_ptr<Expression<T>> rhs{std::move(expressions.top())};
            expressions.pop();
            std::unique_ptr<Expression<T>> lhs{std::move(expressions.top())};
            expressions.pop();
            
            expressions.push(std::make_unique<Concatenation<T>>(*lhs, *rhs));
        }
        else if (c == '+') {
            std::unique_ptr<Expression<T>> rhs{std::move(expressions.top())};
            expressions.pop();
            std::unique_ptr<Expression<T>> lhs{std::move(expressions.top())};
            expressions.pop();
            
            expressions.push(std::make_unique<Union<T>>(*lhs, *rhs));
        }
    }
    
    return std::move(expressions.top());
}

template<typename T>
std::unique_ptr<Expression<T>> parse(const std::string& infix) {
    if (!is_valid(infix)) {
        throw InvalidExpressionException("Invalid expression");
    }
    
    return create<T>(shunting_yard(expand(infix)));
}

#endif /* Parser_hpp */
