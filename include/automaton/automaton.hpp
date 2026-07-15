#ifndef AUTOMATON_AUTOMATON_HPP
#define AUTOMATON_AUTOMATON_HPP

#include <ostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "automaton/state.hpp"
#include "automaton/transition.hpp"

class Automaton {
 public:
    using Alphabet = std::set<char>;

    explicit Automaton(const Alphabet&, const std::vector<State>& = {}, ID initial_state = 0);
    explicit Automaton(char, ID initial_state = 0);

    const std::set<char>& alphabet() const;
    std::vector<State> states() const;
    ID initial_state() const;

    bool empty() const;
    bool deterministic() const;
    bool recognises(const std::string&) const;

    ulong transition_count() const;

    void remove_unreachable_states();
    void remove_epsilons();
    void normalise();
    void convert();

    Automaton operator+(const Automaton&) const;
    Automaton operator*(const Automaton&) const;
    Automaton operator*() const;

    void print(std::ostream&) const;

 private:
    Alphabet alphabet_;
    std::vector<State> states_;
    ID initial_state_;

    void depth_first_search(std::vector<bool>&, ID = 0) const;

    std::unordered_map<ID, ID> get_updated_ids() const;
    Alphabet combine_alphabets(const Automaton&, const Automaton&) const;

    void remove_epsilons_util(ID, ID, std::vector<bool>&, bool&);
};

#endif /* AUTOMATON_AUTOMATON_HPP */
