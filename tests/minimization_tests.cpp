#include <catch2/catch_test_macros.hpp>

#include "automaton/automaton.hpp"
#include "automaton/state.hpp"
#include "test_helpers.hpp"

namespace {

Automaton ending_in_a_dfa() {
    State state_0{false};
    State state_1{true};
    State state_2{false};
    State state_3{true};
    State unreachable{false};

    state_0.add_transition('a', 1);
    state_0.add_transition('b', 2);

    state_1.add_transition('a', 1);
    state_1.add_transition('b', 2);

    state_2.add_transition('a', 3);
    state_2.add_transition('b', 2);

    state_3.add_transition('a', 3);
    state_3.add_transition('b', 2);

    unreachable.add_transition('a', 4);
    unreachable.add_transition('b', 4);

    return Automaton{
        {'a', 'b'},
        {
            state_0,
            state_1,
            state_2,
            state_3,
            unreachable
        },
        0
    };
}

} // namespace

TEST_CASE(
    "minimisation merges equivalent states",
    "[automaton][minimisation]"
) {
    Automaton automaton{ending_in_a_dfa()};

    REQUIRE(automaton.deterministic());
    REQUIRE(automaton.states().size() == 5);

    automaton = automaton.minimized();

    REQUIRE(automaton.deterministic());
    REQUIRE(automaton.states().size() == 2);
    REQUIRE(automaton.transition_count() == 4);
}

TEST_CASE(
    "minimisation preserves the recognized language",
    "[automaton][minimisation]"
) {
    const Automaton original{ending_in_a_dfa()};

    Automaton minimal{original};
    minimal = minimal.minimized();

    test::require_same_language(
        original,
        minimal,
        "ab",
        6
    );
}

TEST_CASE(
    "minimisation can determinize an NFA first",
    "[automaton][minimisation][nfa]"
) {
    const Automaton a{'a'};
    const Automaton b{'b'};
    const Automaton nfa{a + b};

    REQUIRE_FALSE(nfa.deterministic());

    Automaton minimal{nfa};
    minimal = minimal.minimized();

    REQUIRE(minimal.deterministic());

    test::require_same_language(
        nfa,
        minimal,
        "ab",
        5
    );
}

TEST_CASE(
    "minimisation is idempotent",
    "[automaton][minimisation]"
) {
    Automaton once{ending_in_a_dfa()};
    once = once.minimized();

    Automaton twice{once.minimized()};

    REQUIRE(twice.states().size() == once.states().size());
    REQUIRE(twice.transition_count() == once.transition_count());
    REQUIRE(twice.deterministic());

    test::require_same_language(
        once,
        twice,
        "ab",
        6
    );
}

TEST_CASE(
    "minimized returns a new automaton without changing the original",
    "[automaton][minimisation]"
) {
    const Automaton original{ending_in_a_dfa()};
    const Automaton minimal{original.minimized()};

    REQUIRE(original.states().size() == 5);
    REQUIRE(minimal.states().size() == 2);

    test::require_same_language(
        original,
        minimal,
        "ab",
        6
    );
}

TEST_CASE(
    "minimisation handles a single rejecting state",
    "[automaton][minimisation][edge]"
) {
    State state{false};
    state.add_transition('a', 0);

    Automaton automaton{
        {'a'},
        {state},
        0
    };

    automaton = automaton.minimized();

    REQUIRE(automaton.states().size() == 1);
    REQUIRE(automaton.transition_count() == 1);
    REQUIRE(automaton.deterministic());
    REQUIRE(automaton.empty());
}

TEST_CASE(
    "minimisation handles a DFA whose states are all accepting",
    "[automaton][minimisation][edge]"
) {
    State state{true};
    state.add_transition('a', 0);

    Automaton automaton{
        {'a'},
        {state},
        0
    };

    automaton = automaton.minimized();

    REQUIRE(automaton.states().size() == 1);
    REQUIRE(automaton.transition_count() == 1);
    REQUIRE(automaton.deterministic());

    REQUIRE(automaton.recognizes(""));
    REQUIRE(automaton.recognizes("a"));
    REQUIRE(automaton.recognizes("aaaa"));
}

TEST_CASE(
    "minimisation handles an empty alphabet",
    "[automaton][minimisation][edge]"
) {
    Automaton automaton{
        {},
        {State{false}},
        0
    };

    REQUIRE(automaton.deterministic());

    automaton = automaton.minimized();

    REQUIRE(automaton.states().size() == 1);
    REQUIRE(automaton.transition_count() == 0);
    REQUIRE(automaton.deterministic());
}
