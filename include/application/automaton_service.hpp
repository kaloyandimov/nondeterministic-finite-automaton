#ifndef APPLICATION_AUTOMATON_SERVICE_HPP
#define APPLICATION_AUTOMATON_SERVICE_HPP

#include <filesystem>
#include <string_view>
#include <vector>

#include "automaton/automaton.hpp"
#include "export/automaton_dot_exporter.hpp"
#include "persistence/automaton_repository.hpp"
#include "regex/regex_parser.hpp"
#include "storage/storage.hpp"

class AutomatonService {
 public:
    using Id = Storage<Automaton>::Id;

    AutomatonService(
        Storage<Automaton> storage = {},
        AutomatonRepository repository = {},
        RegexParser regex_parser = {},
        AutomatonDotExporter exporter = {});

    std::vector<Id> ids() const;
    const Automaton& get(Id id) const;

    Id create_from_regex(std::string_view expression);
    Id unite(Id lhs, Id rhs);
    Id concatenate(Id lhs, Id rhs);
    Id kleene_star(Id id);
    Id determinize(Id id);

    bool empty(Id id) const;
    bool deterministic(Id id) const;
    bool recognizes(Id id, std::string_view word) const;

    std::vector<Id> load(const std::filesystem::path& path);
    void save(const std::filesystem::path& path) const;
    void save(Id id, const std::filesystem::path& path) const;
    void export_dot(Id id, const std::filesystem::path& path) const;

    void remove(Id id);
    void clear() noexcept;

 private:
    Storage<Automaton> storage_;
    AutomatonRepository repository_;
    RegexParser regex_parser_;
    AutomatonDotExporter exporter_;
};

#endif // APPLICATION_AUTOMATON_SERVICE_HPP
