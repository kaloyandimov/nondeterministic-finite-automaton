#ifndef CONTROLLER_COMMAND_REGISTRY_HPP
#define CONTROLLER_COMMAND_REGISTRY_HPP

#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "controller/command.hpp"

class CommandRegistry {
 public:
    void add(Command command);

    const Command& find(std::string_view name) const;
    const std::vector<Command>& commands() const noexcept;

 private:
    std::vector<Command> commands_;
    std::unordered_map<std::string, std::size_t> indices_;
};

#endif // CONTROLLER_COMMAND_REGISTRY_HPP
