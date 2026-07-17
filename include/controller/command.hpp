#ifndef CONTROLLER_COMMAND_HPP
#define CONTROLLER_COMMAND_HPP

#include <cstddef>
#include <functional>
#include <ostream>
#include <span>
#include <string>
#include <utility>

class AutomatonService;
class CommandRegistry;

struct CommandContext {
    AutomatonService& service;
    const CommandRegistry& registry;
    std::ostream& out;
    bool& running;
};

class Command {
 public:
    using Arguments = std::span<const std::string>;
    using Handler = std::function<void(CommandContext&, Arguments)>;

    Command(
        std::string name,
        std::string arguments,
        std::string description,
        std::size_t min_arguments,
        std::size_t max_arguments,
        Handler handler);

    const std::string& name() const noexcept;
    const std::string& arguments() const noexcept;
    const std::string& description() const noexcept;
    std::size_t min_arguments() const noexcept;
    std::size_t max_arguments() const noexcept;

    std::string title() const;
    void execute(CommandContext& context, Arguments arguments) const;

 private:
    std::string name_;
    std::string arguments_;
    std::string description_;
    std::size_t min_arguments_;
    std::size_t max_arguments_;
    Handler handler_;
};

std::ostream& operator<<(std::ostream& out, const Command& command);

#endif // CONTROLLER_COMMAND_HPP
