#ifndef EXPRESSION_CONCATENATION_HPP
#define EXPRESSION_CONCATENATION_HPP

#include "expression/expression.hpp"

template <typename T>
class Concatenation : public Expression<T> {
   public:
    Concatenation(const Expression<T>&, const Expression<T>&);

    T evaluate() const override;
    std::string to_string() const override;
    std::unique_ptr<Expression<T>> clone() const override;

   private:
    std::unique_ptr<Expression<T>> lhs;
    std::unique_ptr<Expression<T>> rhs;
};

template <typename T>
Concatenation<T>::Concatenation(const Expression<T>& l, const Expression<T>& r) : lhs{l.clone()}, rhs{r.clone()} {
    // nothing
}

template <typename T>
T Concatenation<T>::evaluate() const {
    return lhs->evaluate() * rhs->evaluate();
}

template <typename T>
std::string Concatenation<T>::to_string() const {
    return "(" + lhs->to_string() + "." + rhs->to_string() + ")";
}

template <typename T>
std::unique_ptr<Expression<T>> Concatenation<T>::clone() const {
    return std::make_unique<Concatenation<T>>(*lhs, *rhs);
}

#endif /* EXPRESSION_CONCATENATION_HPP */
