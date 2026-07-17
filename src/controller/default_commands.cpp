#include "controller/default_commands.hpp"

#include <charconv>
#include <iomanip>
#include <ios>
#include <stdexcept>
#include <system_error>
#include <vector>

#include "application/automaton_service.hpp"
#include "controller/command.hpp"

namespace {

AutomatonService::Id parse_id(const std::string& value) {
    AutomatonService::Id id{0};
    const char* const begin{value.data()};
    const char* const end{begin + value.size()};
    const auto [position, error]{std::from_chars(begin, end, id)};

    if (error != std::errc{} || position != end) {
        throw std::invalid_argument{"Invalid ID"};
    }

    return id;
}

const char* boolean_text(bool value) noexcept {
    return value ? "True" : "False";
}

void print_created(std::ostream& out, AutomatonService::Id id) {
    out << "Automaton created. ID: " << id << '\n';
}

} // namespace

CommandRegistry create_default_command_registry() {
    CommandRegistry registry;

    registry.add(Command{
        "list", "", "list all automaton IDs", 0, 0,
        [](CommandContext& context, Command::Arguments arguments) {
            if (context.service.ids().size() == 0) {
                context.out << "No automata\n";
                return;
            }

            for (const AutomatonService::Id id : context.service.ids()) {
                context.out << id << ' ';
            }

            context.out << '\n';
        }});

    registry.add(Command{
        "print", "<id>", "print an automaton", 1, 1,
        [](CommandContext& context, Command::Arguments arguments) {
            context.service.get(parse_id(arguments[0])).print(context.out);
        }});

    registry.add(Command{
        "export", "<id> <path>", "export an automaton as DOT", 2, 2,
        [](CommandContext& context, Command::Arguments arguments) {
            context.service.export_dot(parse_id(arguments[0]), arguments[1]);
            context.out << "Automaton exported successfully\n";
        }});

    registry.add(Command{
        "empty", "<id>", "check whether the language is empty", 1, 1,
        [](CommandContext& context, Command::Arguments arguments) {
            context.out << boolean_text(context.service.empty(parse_id(arguments[0]))) << '\n';
        }});

    registry.add(Command{
        "deterministic", "<id>", "check whether an automaton is deterministic", 1, 1,
        [](CommandContext& context, Command::Arguments arguments) {
            context.out << boolean_text(context.service.deterministic(parse_id(arguments[0]))) << '\n';
        }});

    registry.add(Command{
        "recognize", "<id> <word>", "check whether an automaton recognizes a word", 2, 2,
        [](CommandContext& context, Command::Arguments arguments) {
            context.out << boolean_text(context.service.recognizes(parse_id(arguments[0]), arguments[1])) << '\n';
        }});

    registry.add(Command{
        "regex", "<regex>", "create an automaton from a regular expression", 1, 1,
        [](CommandContext& context, Command::Arguments arguments) {
            print_created(context.out, context.service.create_from_regex(arguments[0]));
        }});

    registry.add(Command{
        "union", "<id1> <id2>", "create the union of two automata", 2, 2,
        [](CommandContext& context, Command::Arguments arguments) {
            print_created(context.out, context.service.unite(parse_id(arguments[0]), parse_id(arguments[1])));
        }});

    registry.add(Command{
        "concat", "<id1> <id2>", "concatenate two automata", 2, 2,
        [](CommandContext& context, Command::Arguments arguments) {
            print_created(context.out, context.service.concatenate(parse_id(arguments[0]), parse_id(arguments[1])));
        }});

    registry.add(Command{
        "kleene", "<id>", "create the Kleene star of an automaton", 1, 1,
        [](CommandContext& context, Command::Arguments arguments) {
            print_created(context.out, context.service.kleene_star(parse_id(arguments[0])));
        }});

    registry.add(Command{
        "determinize", "<id>", "determinize an NFA", 1, 1,
        [](CommandContext& context, Command::Arguments arguments) {
            print_created(context.out, context.service.determinize(parse_id(arguments[0])));
        }});

    registry.add(Command{
        "load", "<filename>", "load automata into the current workspace", 1, 1,
        [](CommandContext& context, Command::Arguments arguments) {
            const std::vector<AutomatonService::Id> ids{context.service.load(arguments[0])};

            context.out << ids.size() << (ids.size() == 1 ? " automaton" : " automata") << " loaded";

            if (!ids.empty()) {
                context.out << ". IDs:";

                for (const AutomatonService::Id id : ids) {
                    context.out << ' ' << id;
                }
            }

            context.out << '\n';
        }});

    registry.add(Command{
        "save", "<filename> | <id> <filename>", "save all automata or one automaton", 1, 2,
        [](CommandContext& context, Command::Arguments arguments) {
            if (arguments.size() == 1) {
                context.service.save(arguments[0]);
                context.out << "Workspace saved successfully\n";
            } else {
                context.service.save(parse_id(arguments[0]), arguments[1]);
                context.out << "Automaton " << arguments[0] << " saved successfully\n";
            }
        }});

    registry.add(Command{
        "remove", "<id>", "remove an automaton from the workspace", 1, 1,
        [](CommandContext& context, Command::Arguments arguments) {
            context.service.remove(parse_id(arguments[0]));
            context.out << "Automaton removed\n";
        }});

    registry.add(Command{
        "clear", "", "remove all automata from the workspace", 0, 0,
        [](CommandContext& context, Command::Arguments) {
            context.service.clear();
            context.out << "Workspace cleared\n";
        }});

    registry.add(Command{
        "help", "", "display this list", 0, 0,
        [](CommandContext& context, Command::Arguments) {
            const std::ios::fmtflags original_flags{context.out.flags()};
            context.out << "Commands available:\n";

            for (const Command& command : context.registry.commands()) {
                context.out << "  " << std::left << std::setw(38) << command.title() << command.description() << '\n';
            }

            context.out.flags(original_flags);
        }});

    registry.add(Command{
        "exit", "", "exit the program", 0, 0,
        [](CommandContext& context, Command::Arguments) {
            context.running = false;
            context.out << "Exiting the program...\n";
        }});

    return registry;
}
