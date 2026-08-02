#include <catch2/catch_test_macros.hpp>

#include <string>
#include <string_view>

#include "automaton/automaton.hpp"
#include "automaton/symbol.hpp"
#include "exception/invalid_expression_exception.hpp"
#include "exception/invalid_symbol_exception.hpp"
#include "exception/unbalanced_brackets_exception.hpp"
#include "regex/regex_parser.hpp"

namespace {

Automaton parse_automaton(std::string_view expression) {
    const RegexParser parser;

    return parser.parse<Automaton>(expression)->evaluate();
}

} // namespace

TEST_CASE(
    "parser creates an atom expression",
    "[regex][atom]"
) {
    const RegexParser parser;
    const auto expression{parser.parse<Automaton>("a")};

    REQUIRE(expression->to_string() == "a");

    const Automaton automaton{expression->evaluate()};

    REQUIRE(automaton.recognizes("a"));

    REQUIRE_FALSE(automaton.recognizes(""));
    REQUIRE_FALSE(automaton.recognizes("b"));
    REQUIRE_FALSE(automaton.recognizes("aa"));
}

TEST_CASE(
    "parser creates a union expression",
    "[regex][union]"
) {
    const RegexParser parser;
    const auto expression{parser.parse<Automaton>("a+b")};

    REQUIRE(expression->to_string() == "(a+b)");

    const Automaton automaton{expression->evaluate()};

    REQUIRE(automaton.recognizes("a"));
    REQUIRE(automaton.recognizes("b"));

    REQUIRE_FALSE(automaton.recognizes(""));
    REQUIRE_FALSE(automaton.recognizes("ab"));
    REQUIRE_FALSE(automaton.recognizes("ba"));
}

TEST_CASE(
    "parser inserts implicit concatenation",
    "[regex][concatenation]"
) {
    const RegexParser parser;
    const auto expression{parser.parse<Automaton>("ab")};

    REQUIRE(expression->to_string() == "(a.b)");

    const Automaton automaton{expression->evaluate()};

    REQUIRE(automaton.recognizes("ab"));

    REQUIRE_FALSE(automaton.recognizes(""));
    REQUIRE_FALSE(automaton.recognizes("a"));
    REQUIRE_FALSE(automaton.recognizes("b"));
    REQUIRE_FALSE(automaton.recognizes("ba"));
}

TEST_CASE(
    "parser supports explicit concatenation",
    "[regex][concatenation]"
) {
    const RegexParser parser;
    const auto expression{parser.parse<Automaton>("a.b")};

    REQUIRE(expression->to_string() == "(a.b)");

    const Automaton automaton{expression->evaluate()};

    REQUIRE(automaton.recognizes("ab"));

    REQUIRE_FALSE(automaton.recognizes("a"));
    REQUIRE_FALSE(automaton.recognizes("b"));
    REQUIRE_FALSE(automaton.recognizes("ba"));
}

TEST_CASE(
    "parser creates a Kleene star expression",
    "[regex][kleene]"
) {
    const RegexParser parser;
    const auto expression{parser.parse<Automaton>("a*")};

    REQUIRE(expression->to_string() == "(a)*");

    const Automaton automaton{expression->evaluate()};

    REQUIRE(automaton.recognizes(""));
    REQUIRE(automaton.recognizes("a"));
    REQUIRE(automaton.recognizes("aa"));
    REQUIRE(automaton.recognizes("aaaaaa"));

    REQUIRE_FALSE(automaton.recognizes("b"));
    REQUIRE_FALSE(automaton.recognizes("ab"));
}

TEST_CASE(
    "Kleene star has higher precedence than concatenation",
    "[regex][precedence]"
) {
    const RegexParser parser;
    const auto expression{parser.parse<Automaton>("ab*")};

    REQUIRE(expression->to_string() == "(a.(b)*)");

    const Automaton automaton{expression->evaluate()};

    REQUIRE(automaton.recognizes("a"));
    REQUIRE(automaton.recognizes("ab"));
    REQUIRE(automaton.recognizes("abb"));
    REQUIRE(automaton.recognizes("abbbbb"));

    REQUIRE_FALSE(automaton.recognizes(""));
    REQUIRE_FALSE(automaton.recognizes("b"));
    REQUIRE_FALSE(automaton.recognizes("aa"));
    REQUIRE_FALSE(automaton.recognizes("ba"));
}

TEST_CASE(
    "concatenation has higher precedence than union",
    "[regex][precedence]"
) {
    const RegexParser parser;
    const auto expression{parser.parse<Automaton>("a+bc")};

    REQUIRE(expression->to_string() == "(a+(b.c))");

    const Automaton automaton{expression->evaluate()};

    REQUIRE(automaton.recognizes("a"));
    REQUIRE(automaton.recognizes("bc"));

    REQUIRE_FALSE(automaton.recognizes(""));
    REQUIRE_FALSE(automaton.recognizes("b"));
    REQUIRE_FALSE(automaton.recognizes("c"));
    REQUIRE_FALSE(automaton.recognizes("abc"));
}

TEST_CASE(
    "parentheses override normal precedence",
    "[regex][precedence][parentheses]"
) {
    const RegexParser parser;
    const auto expression{parser.parse<Automaton>("(a+b)c")};

    REQUIRE(expression->to_string() == "((a+b).c)");

    const Automaton automaton{expression->evaluate()};

    REQUIRE(automaton.recognizes("ac"));
    REQUIRE(automaton.recognizes("bc"));

    REQUIRE_FALSE(automaton.recognizes("a"));
    REQUIRE_FALSE(automaton.recognizes("b"));
    REQUIRE_FALSE(automaton.recognizes("abc"));
}

TEST_CASE(
    "parser supports nested expressions",
    "[regex][complex]"
) {
    const Automaton automaton{
        parse_automaton("(a+b)*abb")
    };

    REQUIRE(automaton.recognizes("abb"));
    REQUIRE(automaton.recognizes("aabb"));
    REQUIRE(automaton.recognizes("babb"));
    REQUIRE(automaton.recognizes("abababb"));
    REQUIRE(automaton.recognizes("bbbbabb"));

    REQUIRE_FALSE(automaton.recognizes(""));
    REQUIRE_FALSE(automaton.recognizes("ab"));
    REQUIRE_FALSE(automaton.recognizes("abba"));
    REQUIRE_FALSE(automaton.recognizes("aba"));
}

TEST_CASE(
    "parser supports epsilon",
    "[regex][epsilon]"
) {
    const RegexParser parser;
    const std::string input{std::string(1, epsilon_symbol) + "+a"};
    const auto expression{parser.parse<Automaton>(input)};

    REQUIRE(expression->to_string() == "(" + input + ")");

    const Automaton automaton{expression->evaluate()};

    REQUIRE(automaton.recognizes(""));
    REQUIRE(automaton.recognizes("a"));

    REQUIRE_FALSE(automaton.recognizes(std::string(1, epsilon_symbol)));
    REQUIRE_FALSE(automaton.recognizes("aa"));
    REQUIRE_FALSE(automaton.recognizes("b"));
}

TEST_CASE(
    "epsilon can participate in concatenation",
    "[regex][epsilon]"
) {
    const Automaton automaton{
        parse_automaton(std::string(1, epsilon_symbol) + "a")
    };

    REQUIRE(automaton.recognizes("a"));

    REQUIRE_FALSE(automaton.recognizes(""));
    REQUIRE_FALSE(automaton.recognizes(std::string(1, epsilon_symbol)));
    REQUIRE_FALSE(automaton.recognizes(std::string(1, epsilon_symbol) + "a"));
}

TEST_CASE(
    "parser supports digits as symbols",
    "[regex][symbols]"
) {
    const Automaton automaton{
        parse_automaton("(a+1)2")
    };

    REQUIRE(automaton.recognizes("a2"));
    REQUIRE(automaton.recognizes("12"));

    REQUIRE_FALSE(automaton.recognizes("a"));
    REQUIRE_FALSE(automaton.recognizes("1"));
    REQUIRE_FALSE(automaton.recognizes("2"));
}

TEST_CASE(
    "parser rejects an empty expression",
    "[regex][invalid]"
) {
    const RegexParser parser;

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>(""),
        InvalidExpressionException
    );
}

TEST_CASE(
    "parser rejects a missing left operand",
    "[regex][invalid]"
) {
    const RegexParser parser;

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("+a"),
        InvalidExpressionException
    );

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>(".a"),
        InvalidExpressionException
    );

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("*a"),
        InvalidExpressionException
    );
}

TEST_CASE(
    "parser rejects a missing right operand",
    "[regex][invalid]"
) {
    const RegexParser parser;

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("a+"),
        InvalidExpressionException
    );

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("a."),
        InvalidExpressionException
    );
}

TEST_CASE(
    "parser rejects consecutive binary operators",
    "[regex][invalid]"
) {
    const RegexParser parser;

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("a++b"),
        InvalidExpressionException
    );

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("a+.b"),
        InvalidExpressionException
    );

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("a..b"),
        InvalidExpressionException
    );
}

TEST_CASE(
    "parser rejects empty parentheses",
    "[regex][invalid][parentheses]"
) {
    const RegexParser parser;

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("()"),
        InvalidExpressionException
    );
}

TEST_CASE(
    "parser rejects an expression ending inside parentheses",
    "[regex][invalid][parentheses]"
) {
    const RegexParser parser;

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("(a+)"),
        InvalidExpressionException
    );
}

TEST_CASE(
    "parser rejects an unmatched opening parenthesis",
    "[regex][invalid][parentheses]"
) {
    const RegexParser parser;

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("(a+b"),
        UnbalancedBracketsException
    );
}

TEST_CASE(
    "parser rejects an unmatched closing parenthesis",
    "[regex][invalid][parentheses]"
) {
    const RegexParser parser;

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("a+b)"),
        UnbalancedBracketsException
    );
}

TEST_CASE(
    "parser rejects unsupported symbols",
    "[regex][invalid][symbols]"
) {
    const RegexParser parser;

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("A"),
        InvalidSymbolException
    );

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("a@b"),
        InvalidSymbolException
    );

    REQUIRE_THROWS_AS(
        parser.parse<Automaton>("a b"),
        InvalidSymbolException
    );
}
