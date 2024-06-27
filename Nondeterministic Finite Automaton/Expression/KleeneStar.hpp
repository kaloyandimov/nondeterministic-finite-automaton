//
//  KleeneStar.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 27.06.24.
//

#ifndef KleeneStar_hpp
#define KleeneStar_hpp

#include "Expression.hpp"

template <typename T>
class KleeneStar : public Expression<T> {
 public:
    KleeneStar(const Expression<T>&);
   
    T evaluate() const override;
    std::string to_string() const override;
    std::unique_ptr<Expression<T>> clone() const override;
   
 private:
    std::unique_ptr<Expression<T>> expr;
};

template <typename T>
KleeneStar<T>::KleeneStar(const Expression<T>& e) : expr{e.clone()} {
    // nothing
}

template <typename T>
T KleeneStar<T>::evaluate() const {
    return *expr->evaluate();
}

template <typename T>
std::string KleeneStar<T>::to_string() const {
    return "(" + expr->to_string() + ")*";
}

template <typename T>
std::unique_ptr<Expression<T>> KleeneStar<T>::clone() const {
    return std::make_unique<KleeneStar<T>>(*expr);
}

#endif /* KleeneStar_hpp */
