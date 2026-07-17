#ifndef EXPRESSION_KLEENE_STAR_HPP
#define EXPRESSION_KLEENE_STAR_HPP

#include <utility>

#include "expression/expression.hpp"

template <typename T>
class KleeneStar : public Expression<T> {
 public:
    explicit KleeneStar(const Expression<T>& expression);
    explicit KleeneStar(std::unique_ptr<Expression<T>> expression) noexcept;

    T evaluate() const override;
    std::string to_string() const override;
    std::unique_ptr<Expression<T>> clone() const override;

 private:
    std::unique_ptr<Expression<T>> expression_;
};

template <typename T>
KleeneStar<T>::KleeneStar(const Expression<T>& expression) : expression_{expression.clone()} {}

template <typename T>
KleeneStar<T>::KleeneStar(std::unique_ptr<Expression<T>> expression) noexcept : expression_{std::move(expression)} {}

template <typename T>
T KleeneStar<T>::evaluate() const {
    return *expression_->evaluate();
}

template <typename T>
std::string KleeneStar<T>::to_string() const {
    return "(" + expression_->to_string() + ")*";
}

template <typename T>
std::unique_ptr<Expression<T>> KleeneStar<T>::clone() const {
    return std::make_unique<KleeneStar<T>>(*expression_);
}

#endif // EXPRESSION_KLEENE_STAR_HPP
