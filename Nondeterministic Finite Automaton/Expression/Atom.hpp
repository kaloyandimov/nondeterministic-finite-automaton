//
//  Atom.hpp
//  Nondeterministic Finite Automaton
//
//  Created by Kaloyan Dimov on 27.06.24.
//

#ifndef Atom_hpp
#define Atom_hpp

#include "Expression.hpp"

template<typename T>
class Atom : public Expression<T> {
 public:
    Atom(char);
    
    T evaluate() const override;
    std::string to_string() const override;
    std::unique_ptr<Expression<T>> clone() const override;
    
 private:
    char symbol;
};

template<typename T>
Atom<T>::Atom(char c) : symbol{c} {
    // nothing
}

template<typename T>
T Atom<T>::evaluate() const {
    return T{symbol};
}

template<typename T>
std::string Atom<T>::to_string() const {
    return std::string{1, symbol};
}

template<typename T>
std::unique_ptr<Expression<T>> Atom<T>::clone() const {
    return std::make_unique<Atom<T>>(symbol);
}

#endif /* Atom_hpp */
