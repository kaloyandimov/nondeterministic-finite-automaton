//
//  ParserFunctions.cpp
//  Project NFA
//
//  Created by Kaloyan Dimov on 31.05.20.
//  Copyright © 2020 Kaloyan Dimov. All rights reserved.
//

#include "ParserFunctions.hpp"

int priority(char symbol) {
    if (symbol == '*') return 3;
    if (symbol == '.') return 2;
    if (symbol == '+') return 1;
    
    return 0;
}

bool is_operator(char symbol) {
    return symbol == '*' || symbol == '.' || symbol == '+';
}

std::string shunting_yard(const std::string& infix) {
    std::string output;
    std::stack<char> stack;
    
    std::for_each(infix.begin(),
                  infix.end(),
                  [&](unsigned char symbol) {
        if (std::isdigit(symbol) || std::islower(symbol)) {
            output += symbol;
        }
        else if (symbol == '(') {
            stack.push(symbol);
        }
        else if (symbol == ')') {
            while (!stack.empty() && stack.top() != '(') {
                output += stack.top();
                stack.pop();
            }
            
            stack.pop();
        }
        else if (is_operator(symbol)) {
            while(!stack.empty() && priority(stack.top()) >= priority(symbol)) {
                output += stack.top();
                stack.pop();
            }
            
            stack.push(symbol);
        }
        else {
            throw std::invalid_argument{
                "Invalid symbol: " + std::string{static_cast<char>(symbol)}
            };
        }
    });

    while(!stack.empty()) {
        output += stack.top();
        stack.pop();
    }
    
    return output;
}

std::unique_ptr<RegExpr> parse(const std::string& postfix) {
    std::stack<std::unique_ptr<RegExpr>> stack;
    
    for (size_t i{0}; i < postfix.length(); ++i) {
        if (std::isdigit(postfix[i]) || std::islower(postfix[i])) {
            stack.push(std::make_unique<Singleton>(postfix[i]));
        }
        else if (postfix[i] == '*') {
            std::unique_ptr<RegExpr> expression{stack.top()->clone()};
            stack.pop();
            
            stack.push(std::make_unique<KleenePlus>(expression));
        }
        else if (postfix[i] == '.') {
            std::unique_ptr<RegExpr> rhs{stack.top()->clone()};
            stack.pop();
            std::unique_ptr<RegExpr> lhs{stack.top()->clone()};
            stack.pop();
            
            stack.push(std::make_unique<Concatenation>(lhs, rhs));
        }
        else if (postfix[i] == '+') {
            std::unique_ptr<RegExpr> rhs{stack.top()->clone()};
            stack.pop();
            std::unique_ptr<RegExpr> lhs{stack.top()->clone()};
            stack.pop();
            
            stack.push(std::make_unique<Union>(lhs, rhs));
        }
    }
    
    return stack.top()->clone();
}

