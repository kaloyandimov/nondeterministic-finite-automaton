#ifndef AUTOMATON_TRANSITION_HPP
#define AUTOMATON_TRANSITION_HPP

#include <compare>
#include <ostream>

using ID = std::size_t;

class Transition {
   public:
    Transition(char, ID);

    char symbol() const;
    ID endpoint() const;

    void set_symbol(char);
    void set_endpoint(ID);

    bool epsilon() const;

    bool operator<(const Transition&) const;
    bool operator==(const Transition&) const = default;

    friend std::ostream& operator<<(std::ostream&, const Transition&);

   private:
    char symbol_;
    ID endpoint_;
};

#endif /* AUTOMATON_TRANSITION_HPP */
