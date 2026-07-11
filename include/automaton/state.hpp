#ifndef AUTOMATON_STATE_HPP
#define AUTOMATON_STATE_HPP

#include <ostream>
#include <vector>

#include "automaton/transition.hpp"

using ulong = std::size_t;

class State {
   public:
    explicit State(bool = false, ID = 0);

    bool accepting() const;
    ID id() const;
    const std::vector<Transition>& transitions() const;

    void set_accepting(bool);
    void set_id(ID);
    void set_transitions(const std::vector<Transition>&);

    bool deterministic() const;

    ulong transition_count() const;

    void add_transition(const Transition&);
    void add_transition(char, ID);
    void add_epsilon_transition(ID);
    void add_to_ids(ulong);

    bool operator==(const State&) const;
    bool operator<(const State&) const;

    friend std::ostream& operator<<(std::ostream&, const State&);

   private:
    bool accepting_;
    ID id_;
    std::vector<Transition> transitions_;
};

#endif /* AUTOMATON_STATE_HPP */
