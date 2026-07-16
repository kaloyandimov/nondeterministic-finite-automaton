#ifndef CONTROLLER_COMMAND_HPP
#define CONTROLLER_COMMAND_HPP

#include <cstddef>
#include <functional>
#include <ostream>
#include <string>
#include <vector>

#include "exception/invalid_argument_count_exception.hpp"

class Controller;

using ulong = std::size_t;
using Function = std::function<void(Controller&, std::vector<std::string>)>;

class Command {
   public:
    Command() = default;
    Command(const std::string&, const std::string&, const std::string&, ulong, const Function&);

    const std::string& name() const;
    const std::string& args() const;
    const std::string& usage() const;
    ulong arg_count() const;

    bool valid() const;
    std::string title() const;
    void execute(Controller&, const std::vector<std::string>&) const;

    operator bool() const;
    void operator()(Controller&, const std::vector<std::string>&) const;

    friend std::ostream& operator<<(std::ostream&, const Command&);

   private:
    std::string name_;
    std::string args_;
    std::string usage_;
    ulong arg_count_;
    Function function_;
};

#endif // CONTROLLER_COMMAND_HPP
