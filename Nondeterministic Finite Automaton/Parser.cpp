//
//  Parser.cpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#include "Parser.hpp"

Parser::Parser(const std::string& input) {
    parse(input);
}

const std::string& Parser::parsed_name() const {
    return name_;
}

const std::vector<std::string>& Parser::parsed_args() const {
    return args_;
}

Parser::operator bool() const {
    return !name_.empty();
}

std::istream& operator>>(std::istream& in, Parser& x) {
    std::string input;
    
    std::getline(in, input);
    
    x.name_.clear();
    x.args_.clear();
    x.parse(input);
    
    return in;
}

void Parser::parse(const std::string& input) {
    std::istringstream ss{input};
    
    ss >> name_;
    
    std::copy(std::istream_iterator<std::string>{ss},
              std::istream_iterator<std::string>{},
              std::back_inserter(args_));
}

bool is_in_alphabet(char c) {
    return std::islower(c) || std::isdigit(c) || c == 'E';
}

bool is_operator(char c) {
    return c == '.' || c == '+' || c == '*';
}

bool is_bracket(char c) {
    return c == '(' || c == ')';
}

bool is_metasymbol(char c) {
    return is_in_alphabet(c) || is_operator(c) || is_bracket(c);
}

bool is_opening_token(char c) {
    return is_in_alphabet(c) || c == '(';
}

bool is_closing_token(char c) {
    return is_in_alphabet(c) || c == ')' || c == '*';
}

int priority(char c) {
    if (c == '*') return 3;
    if (c == '.') return 2;
    if (c == '+') return 1;
    
    return 0;
}

bool only_metasymbols(const std::string& expr) {
    for (char c : expr) {
        if (!is_metasymbol(c)) {
            throw InvalidSymbolException{"'" + std::string{c} + "' is not a metasymbol"};
        }
    }
 
    return true;
}

bool is_balanced(const std::string& expr) {
    int count{0};
 
    for (char c : expr) {
        if (!is_bracket(c)) continue;
        
        c == '(' ? count++ : count--;
        
        if (count < 0) throw UnbalancedBracketsException{"Unbalanced ')'"};
    }
    
    if (count != 0) throw UnbalancedBracketsException{"Unbalanced '('"};
 
    return true;
}

bool is_correct(const std::string& infix, int i, bool valid) {
    if (i == infix.size()) {
        return valid;
    }
    if (is_in_alphabet(infix[i])) {
        return valid ? false : is_correct(infix, i + 1, true);
    }
    if (infix[i] == '*') {
        return valid ? is_correct(infix, i + 1, true) : false;
    }
    if (infix[i] == '.' || infix[i] == '+') {
        return valid ? is_correct(infix, i + 1, false) : false;
    }
    if (is_bracket(infix[i])) {
        return is_correct(infix, i + 1, valid);
    }

    return false;
}

bool is_valid(const std::string& infix) {
    return only_metasymbols(infix) && is_balanced(infix) && is_correct(expand(infix));
}

std::string expand(const std::string& infix) {
    ulong size{infix.size()};
    std::string result;
    
    result.reserve(size);
    result.push_back(infix[0]);
    
    for (ulong i{1}; i < size; i++) {
        if (is_closing_token(infix[i - 1]) && is_opening_token(infix[i])) {
            result.push_back('.');
        }
        
        result.push_back(infix[i]);
    }
    
    return result;
}

std::string shunting_yard(const std::string& infix) {
    std::stack<char> operators;
    std::string postfix;
    
    postfix.reserve(infix.size());
    
    for (char c : infix) {
        if (is_in_alphabet(c)) {
            postfix.push_back(c);
        }
        else if (is_operator(c)) {
            while (!operators.empty() && priority(operators.top()) >= priority(c)) {
                postfix.push_back(operators.top());
                operators.pop();
            }
            operators.push(c);
        }
        else if (c == '(') {
            operators.push(c);
        }
        else if (c == ')') {
            while (!operators.empty() && operators.top() != '(') {
                postfix.push_back(operators.top());
                operators.pop();
            }
            operators.pop();
        }
    }
    
    while (!operators.empty()) {
        postfix.push_back(operators.top());
        operators.pop();
    }
    
    return postfix;
}

std::unique_ptr<Expression> create(const std::string& postfix) {
    std::stack<std::unique_ptr<Expression>> expressions;
    
    for (char c : postfix) {
        if (is_in_alphabet(c)) {
            expressions.push(std::make_unique<Singleton>(c));
        }
        else if (c == '*') {
            std::unique_ptr<Expression> expr{std::move(expressions.top())};
            expressions.pop();
            
            expressions.push(std::make_unique<KleeneClosure>(*expr));
        }
        else if (c == '.') {
            std::unique_ptr<Expression> rhs{std::move(expressions.top())};
            expressions.pop();
            std::unique_ptr<Expression> lhs{std::move(expressions.top())};
            expressions.pop();
            
            expressions.push(std::make_unique<Concatenation>(*lhs, *rhs));
        }
        else if (c == '+') {
            std::unique_ptr<Expression> rhs{std::move(expressions.top())};
            expressions.pop();
            std::unique_ptr<Expression> lhs{std::move(expressions.top())};
            expressions.pop();
            
            expressions.push(std::make_unique<Union>(*lhs, *rhs));
        }
    }
    
    return std::move(expressions.top());
}

std::unique_ptr<Expression> parse(const std::string& infix) {
    if (!is_valid(infix)) {
        throw InvalidExpressionException("Invalid expression");
    }
    
    return create(shunting_yard(expand(infix)));
}
