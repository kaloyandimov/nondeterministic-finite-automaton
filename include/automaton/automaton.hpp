#ifndef AUTOMATON_AUTOMATON_HPP
#define AUTOMATON_AUTOMATON_HPP

#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "automaton/state.hpp"
#include "automaton/transition.hpp"

class Automaton {
   public:
    explicit Automaton(const std::vector<State>& = {}, ID initial_state = 0, ID id = 0);
    explicit Automaton(char, ID initial_state = 0, ID id = 0);

    std::vector<State> states() const;
    ID initial_state() const;
    ID id() const;

    void set_states(const std::vector<State>&);
    void set_initial_state(ID);
    void set_id(ID);

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

    friend std::istream& operator>>(std::istream&, Automaton&);
    friend std::ostream& operator<<(std::ostream&, const Automaton&);

   private:
    std::vector<State> states_;
    ID initial_state_;
    ID id_;
    
    void depth_first_search(std::vector<bool>&, ID = 0) const;

    std::unordered_map<ID, ID> get_updated_ids() const;
    std::set<char> get_alphabet(const std::vector<State>&) const;

    void remove_epsilons_util(ID, ID, std::vector<bool>&, bool&);
};

#endif /* AUTOMATON_AUTOMATON_HPP */
