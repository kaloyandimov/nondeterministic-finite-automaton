#ifndef EXPRESSION_CONCATENATION_HPP
#define EXPRESSION_CONCATENATION_HPP

#include <utility>

#include "expression/expression.hpp"

template <typename T>
class Concatenation : public Expression<T> {
 public:
    Concatenation(const Expression<T>& lhs, const Expression<T>& rhs);
    Concatenation(std::unique_ptr<Expression<T>> lhs, std::unique_ptr<Expression<T>> rhs) noexcept;

    T evaluate() const override;
    std::string to_string() const override;
    std::unique_ptr<Expression<T>> clone() const override;

 private:
    std::unique_ptr<Expression<T>> lhs_;
    std::unique_ptr<Expression<T>> rhs_;
};

template <typename T>
Concatenation<T>::Concatenation(const Expression<T>& lhs, const Expression<T>& rhs) : lhs_{lhs.clone()}, rhs_{rhs.clone()} {}

template <typename T>
Concatenation<T>::Concatenation(std::unique_ptr<Expression<T>> lhs, std::unique_ptr<Expression<T>> rhs) noexcept
    : lhs_{std::move(lhs)}, rhs_{std::move(rhs)} {}

template <typename T>
T Concatenation<T>::evaluate() const {
    return lhs_->evaluate() * rhs_->evaluate();
}

template <typename T>
std::string Concatenation<T>::to_string() const {
    return "(" + lhs_->to_string() + "." + rhs_->to_string() + ")";
}

template <typename T>
std::unique_ptr<Expression<T>> Concatenation<T>::clone() const {
    return std::make_unique<Concatenation<T>>(*lhs_, *rhs_);
}

#endif // EXPRESSION_CONCATENATION_HPP
