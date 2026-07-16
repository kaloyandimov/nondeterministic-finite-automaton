#ifndef EXPRESSION_UNION_HPP
#define EXPRESSION_UNION_HPP

#include "expression/expression.hpp"

template <typename T>
class Union : public Expression<T> {
 public:
    Union(const Expression<T>& lhs, const Expression<T>& rhs);

    T evaluate() const override;
    std::string to_string() const override;
    std::unique_ptr<Expression<T>> clone() const override;

 private:
    std::unique_ptr<Expression<T>> lhs_;
    std::unique_ptr<Expression<T>> rhs_;
};

template <typename T>
Union<T>::Union(const Expression<T>& lhs, const Expression<T>& rhs) : lhs_{lhs.clone()}, rhs_{rhs.clone()} {}

template <typename T>
T Union<T>::evaluate() const {
    return lhs_->evaluate() + rhs_->evaluate();
}

template <typename T>
std::string Union<T>::to_string() const {
    return "(" + lhs_->to_string() + "+" + rhs_->to_string() + ")";
}

template <typename T>
std::unique_ptr<Expression<T>> Union<T>::clone() const {
    return std::make_unique<Union<T>>(*lhs_, *rhs_);
}

#endif // EXPRESSION_UNION_HPP
