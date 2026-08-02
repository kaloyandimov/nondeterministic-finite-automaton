#include "serialization/automaton_serializer.hpp"

#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "automaton/automaton.hpp"
#include "automaton/state.hpp"
#include "automaton/state_id.hpp"
#include "automaton/transition.hpp"

namespace {

std::string read_line(std::istream& in, const char* field_name) {
    std::string line;

    if (!std::getline(in, line)) {
        throw std::runtime_error{std::string{"Could not read "} + field_name};
    }

    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }

    return line;
}

template <typename T>
T parse_value(const std::string& line, const char* field_name) {
    std::istringstream input{line};
    T value{};

    if (!(input >> value)) {
        throw std::runtime_error{std::string{"Invalid "} + field_name};
    }

    input >> std::ws;

    if (!input.eof()) {
        throw std::runtime_error{std::string{"Invalid "} + field_name};
    }

    return value;
}

} // namespace

Automaton AutomatonSerializer::read(std::istream& in) const {
    const std::string alphabet{read_line(in, "alphabet")};
    const StateId initial_state{parse_value<StateId>(read_line(in, "initial state"), "initial state")};
    const std::size_t state_count{parse_value<std::size_t>(read_line(in, "state count"), "state count")};

    std::vector<State> states;
    states.reserve(state_count);

    std::istringstream acceptance_flags{read_line(in, "acceptance flags")};

    for (std::size_t i{0}; i < state_count; i++) {
        bool is_accepting{false};

        if (!(acceptance_flags >> is_accepting)) {
            throw std::runtime_error{"Invalid acceptance flags"};
        }

        states.emplace_back(is_accepting);
    }

    acceptance_flags >> std::ws;

    if (!acceptance_flags.eof()) {
        throw std::runtime_error{"Invalid acceptance flags"};
    }

    if (states.empty() || initial_state >= states.size()) {
        throw std::runtime_error{"Invalid initial state"};
    }

    const std::size_t transition_count{parse_value<std::size_t>(read_line(in, "transition count"), "transition count")};

    for (std::size_t i{0}; i < transition_count; i++) {
        std::istringstream transition_line{read_line(in, "transition")};
        StateId source{0};
        StateId destination{0};
        char symbol{};

        if (!(transition_line >> source >> symbol >> destination)) {
            throw std::runtime_error{"Invalid transition"};
        }

        transition_line >> std::ws;

        if (!transition_line.eof() || source >= states.size() || destination >= states.size()) {
            throw std::runtime_error{"Invalid transition"};
        }

        states[source].add_transition(symbol, destination);
    }

    return Automaton{{alphabet.begin(), alphabet.end()}, std::move(states), initial_state};
}

void AutomatonSerializer::write(std::ostream& out, const Automaton& automaton) const {
    for (const char symbol : automaton.alphabet()) {
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

    for (StateId i{0}; i < automaton.states().size(); i++) {
        for (const Transition& transition : automaton.states()[i].transitions()) {
            out << i << ' ' << transition.symbol() << ' ' << transition.destination() << '\n';
        }
    }
}
