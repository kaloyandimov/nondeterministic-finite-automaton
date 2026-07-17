#include "controller/command_line_parser.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

CommandLine::operator bool() const noexcept {
    return !name.empty();
}

CommandLine parse_command_line(std::string_view input) {
    std::vector<std::string> tokens;
    std::string token;
    char quote{0};
    bool token_started{false};

    const auto finish_token = [&tokens, &token, &token_started]() {
        if (token_started) {
            tokens.push_back(std::move(token));
            token.clear();
            token_started = false;
        }
    };

    for (std::size_t index{0}; index < input.size(); ++index) {
        const char character{input[index]};

        if (quote != 0) {
            if (character == quote) {
                quote = 0;
            } else if (character == '\\' && index + 1 < input.size()) {
                token.push_back(input[++index]);
            } else {
                token.push_back(character);
            }

            token_started = true;
            continue;
        }

        if (character == '"' || character == '\'') {
            quote = character;
            token_started = true;
        } else if (std::isspace(static_cast<unsigned char>(character)) != 0) {
            finish_token();
        } else {
            token.push_back(character);
            token_started = true;
        }
    }

    finish_token();

    if (tokens.empty()) {
        return {};
    }

    CommandLine result;
    result.name = std::move(tokens.front());
    result.arguments.reserve(tokens.size() - 1);

    for (std::size_t index{1}; index < tokens.size(); ++index) {
        result.arguments.push_back(std::move(tokens[index]));
    }

    return result;
}
