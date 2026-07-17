#include "controller/command_registry.hpp"

#include <stdexcept>
#include <string>
#include <utility>

#include "exception/command_not_found_exception.hpp"

void CommandRegistry::add(Command command) {
    if (indices_.contains(command.name())) {
        throw std::logic_error{"Duplicate command: " + command.name()};
    }

    indices_.emplace(command.name(), commands_.size());
    commands_.push_back(std::move(command));
}

const Command& CommandRegistry::find(std::string_view name) const {
    const auto iterator{indices_.find(std::string{name})};

    if (iterator == indices_.end()) {
        throw CommandNotFoundException{"Command not found"};
    }

    return commands_[iterator->second];
}

const std::vector<Command>& CommandRegistry::commands() const noexcept {
    return commands_;
}
