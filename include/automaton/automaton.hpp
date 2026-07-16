#ifndef AUTOMATON_AUTOMATON_HPP
#define AUTOMATON_AUTOMATON_HPP

#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "automaton/state.hpp"
#include "automaton/state_id.hpp"
#include "automaton/transition.hpp"

class Automaton {
 public:
    using Alphabet = std::set<char>;

    explicit Automaton(char symbol);
    Automaton(Alphabet alphabet, std::vector<State> states, StateId initial_state);

    const Alphabet& alphabet() const noexcept;
    const std::vector<State>& states() const noexcept;
    StateId initial_state() const noexcept;

    std::vector<Transition>::size_type transition_count() const noexcept;

    bool empty() const;
    bool deterministic() const;
    bool recognizes(const std::string& word) const;

    void remove_unreachable_states();
    void remove_epsilons();
    void normalize();
    void convert();

    Automaton operator+(const Automaton& other) const;
    Automaton operator*(const Automaton& other) const;
    Automaton operator*() const;

    void print(std::ostream& out) const;

 private:
    Alphabet alphabet_;
    std::vector<State> states_;
    StateId initial_state_;

    static Alphabet combine_alphabets(const Alphabet& lhs, const Alphabet& rhs);
};

#endif // AUTOMATON_AUTOMATON_HPP
