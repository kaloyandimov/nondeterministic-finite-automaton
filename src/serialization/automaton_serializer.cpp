#include "serialization/automaton_serializer.hpp"

Automaton AutomatonSerializer::read(std::istream& in) const {
    ID initial_state_id;
    ulong states_size{};
    std::string input;

    in >> input;

    Automaton::Alphabet alphabet(input.begin(), input.end());

    in >> initial_state_id >> states_size;

    std::vector<State> states;
    states.reserve(states_size);

    bool accepting{};

    for (ulong i{0}; i < states_size; i++) {
        in >> accepting;

        states.emplace_back(accepting, i);
    }

    ulong transition_count{};
    ID head{}, tail{};
    char symbol{};

    in >> transition_count;

    for (ulong i{0}; i < transition_count; i++) {
        in >> head >> symbol >> tail;

        states[head].add_transition(symbol, tail);
    }

    return Automaton{alphabet, std::move(states), initial_state_id};
}

void AutomatonSerializer::write(std::ostream& out, const Automaton& automaton) const {
    auto states = automaton.states();

    for (char c : automaton.alphabet()) {
        out << c;
    }

    out << '\n' << automaton.initial_state() << " " << states.size() << '\n';

    for (const State& state : states) {
        out << state.accepting() << " ";
    }

    out << '\n' << automaton.transition_count() << '\n';

    for (const State& state : states) {
        out << state;
    }
}
