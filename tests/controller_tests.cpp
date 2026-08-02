#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <string>

#include "application/automaton_service.hpp"
#include "automaton/symbol.hpp"
#include "controller/controller.hpp"
#include "controller/default_commands.hpp"

namespace {

struct ControllerResult {
    std::string output;
    std::string error;
};

ControllerResult run_controller(const std::string& commands) {
    std::istringstream input{commands};
    std::ostringstream output;
    std::ostringstream error;

    Controller controller{
        AutomatonService{},
        create_default_command_registry(),
        input,
        output,
        error
    };

    controller.run();

    return {
        output.str(),
        error.str()
    };
}

bool contains(
    const std::string& text,
    const std::string& expected
) {
    return text.find(expected) != std::string::npos;
}

} // namespace

TEST_CASE(
    "controller creates and recognizes an automaton",
    "[controller]"
) {
    const ControllerResult result{
        run_controller(
            "regex a\n"
            "recognize 1 a\n"
            "recognize 1 b\n"
            "exit\n"
        )
    };

    REQUIRE(result.error.empty());
    REQUIRE(contains(result.output, "Automaton created. ID: 1"));
    REQUIRE(contains(result.output, "True"));
    REQUIRE(contains(result.output, "False"));
}

TEST_CASE(
    "controller union uses both supplied IDs",
    "[controller][union]"
) {
    const ControllerResult result{
        run_controller(
            "regex a\n"
            "regex b\n"
            "union 1 2\n"
            "recognize 3 a\n"
            "recognize 3 b\n"
            "recognize 3 ab\n"
            "exit\n"
        )
    };

    REQUIRE(result.error.empty());
    REQUIRE(contains(result.output, "Automaton created. ID: 1"));
    REQUIRE(contains(result.output, "Automaton created. ID: 2"));
    REQUIRE(contains(result.output, "Automaton created. ID: 3"));

    const std::size_t first_true{
        result.output.find("True")
    };

    REQUIRE(first_true != std::string::npos);
    REQUIRE(
        result.output.find("True", first_true + 1) !=
        std::string::npos
    );
    REQUIRE(contains(result.output, "False"));
}

TEST_CASE(
    "controller concatenation uses both supplied IDs",
    "[controller][concatenation]"
) {
    const ControllerResult result{
        run_controller(
            "regex a\n"
            "regex b\n"
            "concat 1 2\n"
            "recognize 3 ab\n"
            "recognize 3 aa\n"
            "recognize 3 ba\n"
            "exit\n"
        )
    };

    REQUIRE(result.error.empty());
    REQUIRE(contains(result.output, "Automaton created. ID: 3"));
    REQUIRE(contains(result.output, "True"));
    REQUIRE(contains(result.output, "False"));
}

TEST_CASE(
    "controller creates Kleene closure",
    "[controller][kleene]"
) {
    const ControllerResult result{
        run_controller(
            "regex a\n"
            "kleene 1\n"
            "recognize 2 " + std::string(1, epsilon_symbol) + "\n"
            "recognize 2 aaaa\n"
            "recognize 2 b\n"
            "exit\n"
        )
    };

    REQUIRE(result.error.empty());
    REQUIRE(contains(result.output, "Automaton created. ID: 2"));
    REQUIRE(contains(result.output, "True"));
    REQUIRE(contains(result.output, "False"));
}

TEST_CASE(
    "controller determinizes an NFA",
    "[controller][determinization]"
) {
    const ControllerResult result{
        run_controller(
            "regex a+b\n"
            "determinize 1\n"
            "deterministic 2\n"
            "recognize 2 a\n"
            "recognize 2 b\n"
            "exit\n"
        )
    };

    REQUIRE(result.error.empty());
    REQUIRE(contains(result.output, "Automaton created. ID: 2"));
    REQUIRE(contains(result.output, "True"));
}

TEST_CASE(
    "controller reports an unknown command",
    "[controller][invalid]"
) {
    const ControllerResult result{
        run_controller(
            "definitely-not-a-command\n"
            "exit\n"
        )
    };

    REQUIRE_FALSE(result.error.empty());
}

TEST_CASE(
    "controller reports an invalid automaton ID",
    "[controller][invalid]"
) {
    const ControllerResult result{
        run_controller(
            "recognise 999 a\n"
            "exit\n"
        )
    };

    REQUIRE_FALSE(result.error.empty());
}

TEST_CASE(
    "controller reports an incorrect argument count",
    "[controller][invalid]"
) {
    const ControllerResult result{
        run_controller(
            "union 1\n"
            "exit\n"
        )
    };

    REQUIRE_FALSE(result.error.empty());
}
