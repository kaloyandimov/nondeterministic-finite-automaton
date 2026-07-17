#ifndef REGEX_REGEX_PARSER_HPP
#define REGEX_REGEX_PARSER_HPP

#include <memory>
#include <stack>
#include <string>
#include <string_view>

#include "exception/invalid_expression_exception.hpp"
#include "expression/atom.hpp"
#include "expression/concatenation.hpp"
#include "expression/expression.hpp"
#include "expression/kleene_star.hpp"
#include "expression/union.hpp"

class RegexParser {
 public:
    template <typename T>
    std::unique_ptr<Expression<T>> parse(std::string_view expression) const;

 private:
    static bool is_symbol(char symbol) noexcept;
    static bool is_operator(char symbol) noexcept;
    static int precedence(char symbol) noexcept;

    static void validate(std::string_view expression);
    static std::string expand(std::string_view expression);
    static std::string to_postfix(std::string_view expression);

    template <typename T>
    static std::unique_ptr<Expression<T>> build(std::string_view postfix);
};

template <typename T>
std::unique_ptr<Expression<T>> RegexParser::parse(std::string_view expression) const {
    validate(expression);

    return build<T>(to_postfix(expand(expression)));
}

template <typename T>
std::unique_ptr<Expression<T>> RegexParser::build(std::string_view postfix) {
    std::stack<std::unique_ptr<Expression<T>>> expressions;

    for (const char token : postfix) {
        if (is_symbol(token)) {
            expressions.push(std::make_unique<Atom<T>>(token));
        } else if (token == '*') {
            std::unique_ptr<Expression<T>> expression{std::move(expressions.top())};
            expressions.pop();

            expressions.push(std::make_unique<KleeneStar<T>>(*expression));
        } else {
            std::unique_ptr<Expression<T>> rhs{std::move(expressions.top())};
            expressions.pop();

            std::unique_ptr<Expression<T>> lhs{std::move(expressions.top())};
            expressions.pop();

            if (token == '+') {
                expressions.push(std::make_unique<Union<T>>(*lhs, *rhs));
            } else {
                expressions.push(std::make_unique<Concatenation<T>>(*lhs, *rhs));
            }
        }
    }

    return std::move(expressions.top());
}

#endif // REGEX_REGEX_PARSER_HPP
