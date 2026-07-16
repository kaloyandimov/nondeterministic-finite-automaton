#include "serialization/automaton_serializer.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "automaton/automaton.hpp"

Automaton AutomatonSerializer::read(std::istream& in) const {
    std::string alphabet;
    std::size_t initial_state{0};
    std::size_t states_size{0};

    in >> alphabet >> initial_state >> states_size;

    std::vector<State> states;
    bool accepting{false};

    states.reserve(states_size);

    for (std::size_t i{0}; i < states_size; i++) {
        in >> accepting;

        states.emplace_back(accepting);
    }

    std::size_t transition_count{0};
    std::size_t head{0};
    std::size_t tail{0};
    char symbol{};

    in >> transition_count;

    for (std::size_t i{0}; i < transition_count; i++) {
        in >> head >> symbol >> tail;

        states[head].add_transition(symbol, tail);
    }

    return Automaton{{alphabet.begin(), alphabet.end()}, std::move(states), initial_state};
}

void AutomatonSerializer::write(std::ostream& out, const Automaton& automaton) const {
    for (char c : automaton.alphabet()) {
        out << c;
    }

    out << '\n' << automaton.initial_state() << ' ' << automaton.states().size() << '\n';

    for (const State& state : automaton.states()) {
        out << state.is_accepting() << ' ';
    }

    out << '\n' << automaton.transition_count() << '\n';

    for (auto i{0}; i < automaton.states().size(); i++) {
        for (const Transition& transition : automaton.states()[i].transitions()) {
            out << i  << ' ' << transition.symbol() << ' ' << transition.destination();
        }
    }
}
