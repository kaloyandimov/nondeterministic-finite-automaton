#ifndef AUTOMATON_STATE_HPP
#define AUTOMATON_STATE_HPP

#include <vector>

#include "automaton/state_id.hpp"
#include "automaton/transition.hpp"

class Automaton;

class State {
 public:
    explicit State(bool is_accepting = false);

    const std::vector<Transition>& transitions() const noexcept;
    bool is_accepting() const noexcept;

    std::vector<Transition>::size_type transition_count() const noexcept;

    void add_transition(const Transition& transition);
    void add_transition(char symbol, StateId destination);
    void add_epsilon_transition(StateId destination);

 private:
    std::vector<Transition> transitions_;
    bool is_accepting_;

    void set_is_accepting(bool is_accepting) noexcept;
    void set_transitions(std::vector<Transition> transitions);

    void shift_ids(StateId offset);

    friend class Automaton;
};

#endif // AUTOMATON_STATE_HPP
