#include "persistence/automaton_repository.hpp"

#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <utility>
#include <vector>

AutomatonRepository::AutomatonRepository(AutomatonSerializer serializer) : serializer_{std::move(serializer)} {}

std::vector<Automaton> AutomatonRepository::load(const std::filesystem::path& path) const {
    std::ifstream file{path};

    if (!file) {
        throw std::runtime_error{"Could not open file: " + path.string()};
    }

    std::size_t count{0};
    file >> count;

    std::vector<Automaton> automata;
    automata.reserve(count);

    for (std::size_t index{0}; index < count; ++index) {
        automata.push_back(serializer_.read(file));
    }

    if (!file) {
        throw std::runtime_error{"Could not read file: " + path.string()};
    }

    return automata;
}

void AutomatonRepository::save(const std::filesystem::path& path, std::span<const Automaton* const> automata) const {
    std::ofstream file{path};

    if (!file) {
        throw std::runtime_error{"Could not open file: " + path.string()};
    }

    file << automata.size() << '\n';

    for (const Automaton* automaton : automata) {
        serializer_.write(file, *automaton);
    }

    if (!file) {
        throw std::runtime_error{"Could not write file: " + path.string()};
    }
}

void AutomatonRepository::save(const std::filesystem::path& path, const Automaton& automaton) const {
    const Automaton* pointer{&automaton};

    save(path, std::span<const Automaton* const>{&pointer, 1});
}
