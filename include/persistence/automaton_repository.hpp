#ifndef PERSISTENCE_AUTOMATON_REPOSITORY_HPP
#define PERSISTENCE_AUTOMATON_REPOSITORY_HPP

#include <filesystem>
#include <span>
#include <vector>

#include "automaton/automaton.hpp"
#include "serialization/automaton_serializer.hpp"

class AutomatonRepository {
 public:
    AutomatonRepository() = default;
    explicit AutomatonRepository(AutomatonSerializer serializer);

    std::vector<Automaton> load(const std::filesystem::path& path) const;

    void save(const std::filesystem::path& path, std::span<const Automaton* const> automata) const;
    void save(const std::filesystem::path& path, const Automaton& automaton) const;

 private:
    AutomatonSerializer serializer_;
};

#endif // PERSISTENCE_AUTOMATON_REPOSITORY_HPP
