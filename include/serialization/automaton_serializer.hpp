#ifndef SERIALIZATION_AUTOMATON_SERIALIZER_HPP
#define SERIALIZATION_AUTOMATON_SERIALIZER_HPP

#include <istream>
#include <ostream>

class Automaton;

class AutomatonSerializer {
 public:
    Automaton read(std::istream& in) const;

    void write(std::ostream& out, const Automaton& automaton) const;
};

#endif // SERIALIZATION_AUTOMATON_SERIALIZER_HPP
