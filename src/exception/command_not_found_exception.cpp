#include "exception/command_not_found_exception.hpp"

CommandNotFoundException::CommandNotFoundException(const std::string& what_arg) : CustomException{what_arg} {}
