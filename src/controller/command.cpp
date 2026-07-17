#include "controller/command.hpp"

#include <utility>

#include "exception/invalid_argument_count_exception.hpp"

Command::Command(
    std::string name,
    std::string arguments,
    std::string description,
    std::size_t min_arguments,
    std::size_t max_arguments,
    Handler handler)
    : name_{std::move(name)},
      arguments_{std::move(arguments)},
      description_{std::move(description)},
      min_arguments_{min_arguments},
      max_arguments_{max_arguments},
      handler_{std::move(handler)} {}

const std::string& Command::name() const noexcept {
    return name_;
}

const std::string& Command::arguments() const noexcept {
    return arguments_;
}

const std::string& Command::description() const noexcept {
    return description_;
}

std::size_t Command::min_arguments() const noexcept {
    return min_arguments_;
}

std::size_t Command::max_arguments() const noexcept {
    return max_arguments_;
}

std::string Command::title() const {
    return arguments_.empty() ? name_ : name_ + ' ' + arguments_;
}

void Command::execute(CommandContext& context, Arguments arguments) const {
    if (arguments.size() < min_arguments_) {
        throw InvalidArgumentCountException{"Too few arguments"};
    }

    if (arguments.size() > max_arguments_) {
        throw InvalidArgumentCountException{"Too many arguments"};
    }

    handler_(context, arguments);
}

std::ostream& operator<<(std::ostream& out, const Command& command) {
    return out << command.title() << " - " << command.description();
}
