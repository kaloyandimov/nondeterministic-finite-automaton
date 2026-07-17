#include "serialization/automaton_serializer.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "automaton/automaton.hpp"
#include "automaton/state.hpp"
#include "automaton/state_id.hpp"
#include "automaton/transition.hpp"

Automaton AutomatonSerializer::read(std::istream& in) const {
    std::string alphabet;
    StateId initial_state{0};
    std::size_t states_size{0};

    in >> alphabet >> initial_state >> states_size;

    std::vector<State> states;
    states.reserve(states_size);

    for (std::size_t i{0}; i < states_size; i++) {
        bool accepting{false};

        in >> accepting;

        states.emplace_back(accepting);
    }

    std::size_t transition_count{0};
    
    in >> transition_count;

    for (std::size_t i{0}; i < transition_count; i++) {
        StateId source{0};
        StateId destination{0};
        char symbol{};

        in >> source >> symbol >> destination;

        states[source].add_transition(symbol, destination);
    }

    return Automaton{{alphabet.begin(), alphabet.end()}, std::move(states), initial_state};
}

void AutomatonSerializer::write(std::ostream& out, const Automaton& automaton) const {
    for (char symbol : automaton.alphabet()) {
        out << symbol;
    }

    out << '\n';
    out << automaton.initial_state() << '\n';
    out << automaton.states().size() << '\n';

    for (const State& state : automaton.states()) {
        out << state.is_accepting() << ' ';
    }

    out << '\n';
    out << automaton.transition_count() << '\n';

    for (auto i{0}; i < automaton.states().size(); i++) {
        for (const Transition& transition : automaton.states()[i].transitions()) {
            out << i  << ' ' << transition.symbol() << ' ' << transition.destination() << '\n';
        }
    }
}
