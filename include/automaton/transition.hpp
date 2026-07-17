#ifndef AUTOMATON_TRANSITION_HPP
#define AUTOMATON_TRANSITION_HPP

#include <compare>

#include "automaton/state_id.hpp"

class State;
class Automaton;

class Transition {
 public:
    Transition(char symbol, StateId destination) noexcept;

    char symbol() const noexcept;
    StateId destination() const noexcept;

    bool is_epsilon() const noexcept;

    bool operator<(const Transition& transition) const noexcept;
    bool operator==(const Transition& transition) const noexcept = default;

 private:
    char symbol_;
    StateId destination_;

    friend class State;
    friend class Automaton;
};

#endif // AUTOMATON_TRANSITION_HPP
