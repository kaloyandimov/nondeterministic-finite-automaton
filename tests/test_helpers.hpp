#ifndef TESTS_TEST_HELPERS_HPP
#define TESTS_TEST_HELPERS_HPP

#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

#include "automaton/automaton.hpp"

namespace test {

inline void append_words(
    std::vector<std::string>& words,
    std::string prefix,
    std::string_view alphabet,
    std::size_t remaining_length
) {
    words.push_back(prefix);

    if (remaining_length == 0) {
        return;
    }

    for (const char symbol : alphabet) {
        append_words(
            words,
            prefix + symbol,
            alphabet,
            remaining_length - 1
        );
    }
}

inline std::vector<std::string> words_over(
    std::string_view alphabet,
    std::size_t maximum_length
) {
    std::vector<std::string> words;

    append_words(words, "", alphabet, maximum_length);

    return words;
}

inline void require_same_language(
    const Automaton& lhs,
    const Automaton& rhs,
    std::string_view alphabet,
    std::size_t maximum_length
) {
    for (const std::string& word : words_over(alphabet, maximum_length)) {
        INFO("Word: " << (word.empty() ? "<epsilon>" : word));

        REQUIRE(lhs.recognizes(word) == rhs.recognizes(word));
    }
}

} // namespace test

#endif // TESTS_TEST_HELPERS_HPP
