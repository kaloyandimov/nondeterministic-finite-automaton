#ifndef EXCEPTION_INVALID_EXPRESSION_EXCEPTION_HPP
#define EXCEPTION_INVALID_EXPRESSION_EXCEPTION_HPP

#include "exception/custom_exception.hpp"

class InvalidExpressionException : public CustomException {
 public:
    using CustomException::CustomException;
};

#endif // EXCEPTION_INVALID_EXPRESSION_EXCEPTION_HPP
