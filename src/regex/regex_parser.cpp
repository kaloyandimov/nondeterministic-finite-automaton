#include "regex/regex_parser.hpp"

#include <cstddef>
#include <stack>
#include <string>

#include "automaton/symbol.hpp"
#include "exception/invalid_expression_exception.hpp"
#include "exception/invalid_symbol_exception.hpp"
#include "exception/unbalanced_brackets_exception.hpp"

bool RegexParser::is_symbol(char symbol) noexcept {
    return is_input_symbol(symbol) || symbol == epsilon_symbol;
}

bool RegexParser::is_operator(char symbol) noexcept {
    return symbol == '+' || symbol == '.' || symbol == '*';
}

int RegexParser::precedence(char symbol) noexcept {
    if (symbol == '*') {
        return 3;
    }

    if (symbol == '.') {
        return 2;
    }

    if (symbol == '+') {
        return 1;
    }

    return 0;
}

void RegexParser::validate(std::string_view expression) {
    if (expression.empty()) {
        throw InvalidExpressionException{"Regular expression cannot be empty"};
    }

    std::size_t open_brackets{0};
    bool expects_operand{true};

    for (const char token : expression) {
        if (is_symbol(token)) {
            expects_operand = false;
            continue;
        }

        if (token == '(') {
            open_brackets++;
            expects_operand = true;
            continue;
        }

        if (token == ')') {
            if (open_brackets == 0) {
                throw UnbalancedBracketsException{"Unbalanced ')'"};
            }

            if (expects_operand) {
                throw InvalidExpressionException{"Invalid expression before ')'"};
            }

            open_brackets--;
            expects_operand = false;
            continue;
        }

        if (token == '*') {
            if (expects_operand) {
                throw InvalidExpressionException{"'*' has no operand"};
            }

            continue;
        }

        if (token == '+' || token == '.') {
            if (expects_operand) {
                throw InvalidExpressionException{"Binary operator has no left operand"};
            }

            expects_operand = true;
            continue;
        }

        throw InvalidSymbolException{"'" + std::string(1, token) + "' is not a valid symbol"};
    }

    if (open_brackets != 0) {
        throw UnbalancedBracketsException{"Unbalanced '('"};
    }

    if (expects_operand) {
        throw InvalidExpressionException{"Expression ends with an operator"};
    }
}

std::string RegexParser::expand(std::string_view expression) {
    std::string result;
    result.reserve(expression.size());

    for (std::size_t i{0}; i < expression.size(); i++) {
        if (i > 0) {
            const char previous{expression[i - 1]};
            const char current{expression[i]};
            const bool previous_closes{is_symbol(previous) || previous == ')' || previous == '*'};
            const bool current_opens{is_symbol(current) || current == '('};

            if (previous_closes && current_opens) {
                result.push_back('.');
            }
        }

        result.push_back(expression[i]);
    }

    return result;
}

std::string RegexParser::to_postfix(std::string_view expression) {
    std::stack<char> operators;
    std::string postfix;
    postfix.reserve(expression.size());

    for (const char token : expression) {
        if (is_symbol(token)) {
            postfix.push_back(token);
        } else if (token == '*') {
            postfix.push_back(token);
        } else if (token == '(') {
            operators.push(token);
        } else if (token == ')') {
            while (operators.top() != '(') {
                postfix.push_back(operators.top());
                operators.pop();
            }

            operators.pop();
        } else if (is_operator(token)) {
            while (!operators.empty() && operators.top() != '(' && precedence(operators.top()) >= precedence(token)) {
                postfix.push_back(operators.top());
                operators.pop();
            }

            operators.push(token);
        }
    }

    while (!operators.empty()) {
        postfix.push_back(operators.top());
        operators.pop();
    }

    return postfix;
}
