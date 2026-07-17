#ifndef CONTROLLER_COMMAND_LINE_PARSER_HPP
#define CONTROLLER_COMMAND_LINE_PARSER_HPP

#include <string>
#include <string_view>
#include <vector>

struct CommandLine {
    std::string name;
    std::vector<std::string> arguments;

    explicit operator bool() const noexcept;
};

CommandLine parse_command_line(std::string_view input);

#endif // CONTROLLER_COMMAND_LINE_PARSER_HPP
