#include <catch2/catch_test_macros.hpp>

#include <string>

#include "automaton/automaton.hpp"
#include "automaton/state.hpp"
#include "automaton/symbol.hpp"

TEST_CASE(
    "single-symbol automaton recognizes exactly one symbol",
    "[automaton][recognition]"
) {
    const Automaton automaton{'a'};

    REQUIRE(automaton.recognizes("a"));

    REQUIRE_FALSE(automaton.recognizes(""));
    REQUIRE_FALSE(automaton.recognizes("b"));
    REQUIRE_FALSE(automaton.recognizes("aa"));
    REQUIRE_FALSE(automaton.recognizes("ab"));

    REQUIRE(automaton.alphabet() == Automaton::Alphabet{'a'});
}

TEST_CASE(
    "epsilon automaton recognizes only the empty word",
    "[automaton][recognition][epsilon]"
) {
    const Automaton automaton{epsilon_symbol};

    REQUIRE(automaton.recognizes(""));
    REQUIRE(automaton.alphabet().empty());

    REQUIRE_FALSE(automaton.recognizes("a"));
    REQUIRE_FALSE(automaton.recognizes(std::string(1, epsilon_symbol)));
}

TEST_CASE(
    "recognition terminates when epsilon transitions contain a cycle",
    "[automaton][recognition][epsilon]"
) {
    State initial{false};
    State accepting{true};

    initial.add_epsilon_transition(0);
    initial.add_epsilon_transition(1);

    const Automaton automaton{
        {},
        {initial, accepting},
        0
    };

    REQUIRE(automaton.recognizes(""));
    REQUIRE_FALSE(automaton.recognizes("a"));
}

TEST_CASE(
    "union recognizes either operand",
    "[automaton][union]"
) {
    const Automaton lhs{'a'};
    const Automaton rhs{'b'};
    const Automaton result{lhs + rhs};

    REQUIRE(result.recognizes("a"));
    REQUIRE(result.recognizes("b"));

    REQUIRE_FALSE(result.recognizes(""));
    REQUIRE_FALSE(result.recognizes("ab"));
    REQUIRE_FALSE(result.recognizes("ba"));
    REQUIRE_FALSE(result.recognizes("aa"));
}

TEST_CASE(
    "concatenation recognizes operands in order",
    "[automaton][concatenation]"
) {
    const Automaton lhs{'a'};
    const Automaton rhs{'b'};
    const Automaton result{lhs * rhs};

    REQUIRE(result.recognizes("ab"));

    REQUIRE_FALSE(result.recognizes(""));
    REQUIRE_FALSE(result.recognizes("a"));
    REQUIRE_FALSE(result.recognizes("b"));
    REQUIRE_FALSE(result.recognizes("ba"));
    REQUIRE_FALSE(result.recognizes("abb"));
}

TEST_CASE(
    "Kleene closure recognizes zero or more repetitions",
    "[automaton][kleene]"
) {
    const Automaton operand{'a'};
    const Automaton result{*operand};

    REQUIRE(result.recognizes(""));
    REQUIRE(result.recognizes("a"));
    REQUIRE(result.recognizes("aa"));
    REQUIRE(result.recognizes("aaaaaa"));

    REQUIRE_FALSE(result.recognizes("b"));
    REQUIRE_FALSE(result.recognizes("ab"));
    REQUIRE_FALSE(result.recognizes("ba"));
}

TEST_CASE(
    "empty detects an automaton with no accepting path",
    "[automaton][empty]"
) {
    State initial{false};
    State dead{false};

    initial.add_transition('a', 1);
    dead.add_transition('a', 1);

    const Automaton automaton{
        {'a'},
        {initial, dead},
        0
    };

    REQUIRE(automaton.empty());
    REQUIRE_FALSE(automaton.recognizes(""));
    REQUIRE_FALSE(automaton.recognizes("a"));
    REQUIRE_FALSE(automaton.recognizes("aaaa"));
}

TEST_CASE(
    "empty returns false when an accepting state is reachable",
    "[automaton][empty]"
) {
    State initial{false};
    State accepting{true};

    initial.add_transition('a', 1);
    accepting.add_transition('a', 1);

    const Automaton automaton{
        {'a'},
        {initial, accepting},
        0
    };

    REQUIRE_FALSE(automaton.empty());
    REQUIRE(automaton.recognizes("a"));
    REQUIRE(automaton.recognizes("aaaa"));
}
