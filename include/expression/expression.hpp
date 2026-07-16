#ifndef EXPRESSION_EXPRESSION_HPP
#define EXPRESSION_EXPRESSION_HPP

#include <memory>
#include <ostream>
#include <string>

template <typename T>
class Expression {
 public:
    Expression() = default;
    Expression(const Expression<T>& other) = delete;
    Expression& operator=(const Expression<T>& other) = delete;
    Expression(Expression<T>&& other) = default;
    Expression& operator=(Expression<T>&& other) = default;
    virtual ~Expression() = default;

    virtual T evaluate() const = 0;
    virtual std::string to_string() const = 0;
    virtual std::unique_ptr<Expression<T>> clone() const = 0;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const Expression<T>& expression) {
    return out << expression.to_string();
}

#endif // EXPRESSION_EXPRESSION_HPP
