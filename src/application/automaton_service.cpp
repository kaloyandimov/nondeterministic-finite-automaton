#include "application/automaton_service.hpp"

#include <functional>
#include <utility>
#include <vector>

AutomatonService::AutomatonService(
    Storage<Automaton> storage,
    AutomatonRepository repository,
    RegexParser regex_parser,
    AutomatonDotExporter exporter)
    : storage_{std::move(storage)},
      repository_{std::move(repository)},
      regex_parser_{std::move(regex_parser)},
      exporter_{std::move(exporter)} {}

std::vector<AutomatonService::Id> AutomatonService::ids() const {
    std::vector<Id> result;
    result.reserve(storage_.size());

    for (const auto& [id, automaton] : storage_) {
        static_cast<void>(automaton);
        result.push_back(id);
    }

    return result;
}

const Automaton& AutomatonService::get(Id id) const {
    return storage_.get(id);
}

AutomatonService::Id AutomatonService::create_from_regex(std::string_view expression) {
    const auto parsed_expression{regex_parser_.parse<Automaton>(expression)};

    return storage_.add(parsed_expression->evaluate());
}

AutomatonService::Id AutomatonService::unite(Id lhs, Id rhs) {
    return storage_.add(storage_.get(lhs) + storage_.get(rhs));
}

AutomatonService::Id AutomatonService::concatenate(Id lhs, Id rhs) {
    return storage_.add(storage_.get(lhs) * storage_.get(rhs));
}

AutomatonService::Id AutomatonService::kleene_star(Id id) {
    return storage_.add(*storage_.get(id));
}

AutomatonService::Id AutomatonService::determinize(Id id) {
    return storage_.add(storage_.get(id).determinized());
}

bool AutomatonService::empty(Id id) const {
    return storage_.get(id).empty();
}

bool AutomatonService::deterministic(Id id) const {
    return storage_.get(id).deterministic();
}

bool AutomatonService::recognizes(Id id, std::string_view word) const {
    return storage_.get(id).recognizes(word);
}

std::vector<AutomatonService::Id> AutomatonService::load(const std::filesystem::path& path) {
    std::vector<Automaton> automata{repository_.load(path)};
    std::vector<Id> loaded_ids;
    loaded_ids.reserve(automata.size());

    for (Automaton& automaton : automata) {
        loaded_ids.push_back(storage_.add(std::move(automaton)));
    }

    return loaded_ids;
}

void AutomatonService::save(const std::filesystem::path& path) const {
    std::vector<const Automaton*> automata;
    automata.reserve(storage_.size());

    for (const auto& [id, automaton] : storage_) {
        static_cast<void>(id);
        automata.push_back(&automaton);
    }

    repository_.save(path, automata);
}

void AutomatonService::save(Id id, const std::filesystem::path& path) const {
    repository_.save(path, storage_.get(id));
}

void AutomatonService::export_dot(Id id, const std::filesystem::path& path) const {
    exporter_.export_to_file(storage_.get(id), path);
}

void AutomatonService::remove(Id id) {
    storage_.remove(id);
}

void AutomatonService::clear() noexcept {
    storage_.clear();
}
