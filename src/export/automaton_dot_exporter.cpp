#include "export/automaton_dot_exporter.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "automaton/automaton.hpp"
#include "automaton/state.hpp"
#include "automaton/transition.hpp"

std::string AutomatonDotExporter::to_string(const Automaton& automaton) const {
    std::ostringstream out;

    out << "digraph automaton {\n";
    out << "    rankdir=LR;\n";
    out << "    node [shape=circle];\n\n";

    // Invisible start node.
    out << "    __start [shape=point];\n";
    out << "    __start -> "
        << automaton.initial_state()
        << ";\n\n";

    for (const State& state : automaton.states()) {
        out << "    " << state.id();

        if (state.accepting()) {
            out << " [shape=doublecircle]";
        }

        out << ";\n";
    }

    out << '\n';

    for (const State& state : automaton.states()) {
        for (const Transition& transition : state.transitions()) {
            const std::string label{
                transition.epsilon()
                    ? "ε"
                    : std::string{transition.symbol()}
            };

            out << "    "
                << state.id()
                << " -> "
                << transition.endpoint()
                << " [label=\""
                << escape(label)
                << "\"];\n";
        }
    }

    out << "}\n";

    return out.str();
}

void AutomatonDotExporter::export_to_file(const Automaton& automaton, const std::filesystem::path& path) const {
    std::ofstream file{path};

    if (!file) {
        throw std::runtime_error("Could not open DOT file: " + path.string());
    }

    file << to_string(automaton);

    if (!file) {
        throw std::runtime_error("Could not write DOT file: " + path.string());
    }
}

std::string AutomatonDotExporter::escape(const std::string& text) {
    std::string result;
    result.reserve(text.size());

    for (const char ch : text) {
        switch (ch) {
            case '\\':
                result += "\\\\";
                break;

            case '"':
                result += "\\\"";
                break;

            case '\n':
                result += "\\n";
                break;

            default:
                result += ch;
        }
    }

    return result;
}
