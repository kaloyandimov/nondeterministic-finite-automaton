#ifndef EXCEPTION_INVALID_ARGUMENT_COUNT_EXCEPTION_HPP
#define EXCEPTION_INVALID_ARGUMENT_COUNT_EXCEPTION_HPP

#include "exception/custom_exception.hpp"

class InvalidArgumentCountException : public CustomException {
 public:
    using CustomException::CustomException;
};

#endif // EXCEPTION_INVALID_ARGUMENT_COUNT_EXCEPTION_HPP
