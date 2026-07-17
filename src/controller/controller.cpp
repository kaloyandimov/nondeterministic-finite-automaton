#include "controller/controller.hpp"

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include "controller/command.hpp"
#include "controller/command_line_parser.hpp"
#include "exception/custom_exception.hpp"

Controller::Controller(
    AutomatonService service,
    CommandRegistry commands,
    std::istream& in,
    std::ostream& out,
    std::ostream& err)
    : service_{std::move(service)},
      commands_{std::move(commands)},
      in_{in},
      out_{out},
      err_{err} {}

void Controller::run() {
    std::string input;

    running_ = true;

    while (running_) {
        out_ << "> ";

        if (!std::getline(in_, input)) {
            out_ << '\n';
            break;
        }

        const CommandLine command_line{parse_command_line(input)};

        if (!command_line) {
            continue;
        }

        try {
            CommandContext context{service_, commands_, out_, running_};
            commands_.find(command_line.name).execute(context, command_line.arguments);
        } catch (const std::invalid_argument&) {
            err_ << "Invalid ID\n";
        } catch (const std::out_of_range&) {
            err_ << "ID not found\n";
        } catch (const std::exception& exception) {
            err_ << exception.what() << '\n';
        }
    }
}
