#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string>

#include "automaton/automaton.hpp"
#include "automaton/symbol.hpp"
#include "regex/regex_parser.hpp"
#include "serialization/automaton_serializer.hpp"
#include "test_helpers.hpp"

namespace {

Automaton parse_automaton(std::string_view expression) {
    const RegexParser parser;

    return parser.parse<Automaton>(expression)->evaluate();
}

Automaton round_trip(const Automaton& automaton) {
    AutomatonSerializer serializer;
    std::stringstream stream;

    serializer.write(stream, automaton);

    return serializer.read(stream);
}

} // namespace

TEST_CASE(
    "serializer preserves a nontrivial automaton",
    "[serialization]"
) {
    const Automaton original{
        parse_automaton("(a+b)*abb")
    };

    const Automaton restored{round_trip(original)};

    REQUIRE(restored.alphabet() == original.alphabet());
    REQUIRE(restored.states().size() == original.states().size());
    REQUIRE(restored.initial_state() == original.initial_state());
    REQUIRE(restored.transition_count() == original.transition_count());

    test::require_same_language(
        original,
        restored,
        "ab",
        6
    );
}

TEST_CASE(
    "serializer preserves epsilon transitions",
    "[serialization][epsilon]"
) {
    const Automaton original{
        parse_automaton(std::string{epsilon_symbol} + "+a")
    };

    const Automaton restored{round_trip(original)};

    REQUIRE(restored.recognizes(""));
    REQUIRE(restored.recognizes("a"));
    REQUIRE_FALSE(restored.recognizes("aa"));

    test::require_same_language(
        original,
        restored,
        "ab",
        5
    );
}

TEST_CASE(
    "serializer preserves a deterministic automaton",
    "[serialization][deterministic]"
) {
    const Automaton nfa{
        parse_automaton("(a+b)*abb")
    };

    const Automaton original{nfa.determinized()};
    const Automaton restored{round_trip(original)};

    REQUIRE(original.deterministic());
    REQUIRE(restored.deterministic());

    test::require_same_language(
        original,
        restored,
        "ab",
        6
    );
}

TEST_CASE(
    "serializer preserves an empty alphabet",
    "[serialization][epsilon]"
) {
    const Automaton original{epsilon_symbol};
    const Automaton restored{round_trip(original)};

    REQUIRE(restored.alphabet().empty());
    REQUIRE(restored.recognizes(""));
    REQUIRE_FALSE(restored.recognizes("a"));
}
