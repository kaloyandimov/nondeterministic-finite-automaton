#ifndef EXCEPTION_COMMAND_NOT_FOUND_EXCEPTION_HPP
#define EXCEPTION_COMMAND_NOT_FOUND_EXCEPTION_HPP

#include "exception/custom_exception.hpp"

class CommandNotFoundException : public CustomException {
 public:
    using CustomException::CustomException;
};

#endif // EXCEPTION_COMMAND_NOT_FOUND_EXCEPTION_HPP
