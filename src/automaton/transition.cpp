#include "automaton/transition.hpp"

#include <tuple>

#include "automaton/symbol.hpp"

Transition::Transition(char symbol, StateId destination) noexcept : symbol_{symbol}, destination_{destination} {}

char Transition::symbol() const noexcept {
    return symbol_;
}

StateId Transition::destination() const noexcept {
    return destination_;
}

bool Transition::is_epsilon() const noexcept {
    return symbol_ == epsilon_symbol;
}

bool Transition::operator<(const Transition& other) const noexcept {
    return std::tie(destination_, symbol_) < std::tie(other.destination_, other.symbol_);
}
