//
//  Union.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 22.07.21.
//

#ifndef Union_hpp
#define Union_hpp

#include "Expression.hpp"

template<typename T>
class Union : public Expression<T> {
 public:
    Union(const Expression<T>&, const Expression<T>&);
    
    T evaluate() const override;
    std::string to_string() const override;
    std::unique_ptr<Expression<T>> clone() const override;
    
 private:
    std::unique_ptr<Expression<T>> lhs;
    std::unique_ptr<Expression<T>> rhs;
};

template<typename T>
Union<T>::Union(const Expression<T>& l, const Expression<T>& r) : lhs{l.clone()}, rhs{r.clone()} {
    // nothing
}

template<typename T>
T Union<T>::evaluate() const {
    return lhs->evaluate() + rhs->evaluate();
}

template<typename T>
std::string Union<T>::to_string() const {
    return "(" + lhs->to_string() + "+" + rhs->to_string() + ")";
}

template<typename T>
std::unique_ptr<Expression<T>> Union<T>::clone() const {
    return std::make_unique<Union<T>>(*lhs, *rhs);
}

#endif /* Union_hpp */
