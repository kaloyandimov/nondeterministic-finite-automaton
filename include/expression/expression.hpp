#ifndef EXPRESSION_EXPRESSION_HPP
#define EXPRESSION_EXPRESSION_HPP

#include <memory>
#include <ostream>
#include <string>

template <typename T>
class Expression {
   public:
    Expression() = default;
    Expression(const Expression<T>&) = delete;
    Expression& operator=(const Expression<T>&) = delete;
    Expression(Expression<T>&&) = default;
    Expression& operator=(Expression<T>&&) = default;
    virtual ~Expression() = default;

    virtual T evaluate() const = 0;
    virtual std::string to_string() const = 0;
    virtual std::unique_ptr<Expression<T>> clone() const = 0;
};

template <typename T>
std::ostream& operator<<(std::ostream& out, const Expression<T>& e) {
    return out << e.to_string();
}

#endif /* EXPRESSION_EXPRESSION_HPP */
