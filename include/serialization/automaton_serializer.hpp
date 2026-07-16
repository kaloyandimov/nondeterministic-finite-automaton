#ifndef SERIALIZATION_AUTOMATON_SERIALIZER_HPP
#define SERIALIZATION_AUTOMATON_SERIALIZER_HPP

#include <istream>
#include <ostream>

class Automaton;

class AutomatonSerializer {
 public:
    Automaton read(std::istream&) const;

    void write(std::ostream&, const Automaton&) const;
};

#endif // SERIALIZATION_AUTOMATON_SERIALIZER_HPP
