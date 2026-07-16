#ifndef EXCEPTION_INVALID_SYMBOL_EXCEPTION_HPP
#define EXCEPTION_INVALID_SYMBOL_EXCEPTION_HPP

#include "exception/custom_exception.hpp"

class InvalidSymbolException : public CustomException {
 public:
    using CustomException::CustomException;
};

#endif // EXCEPTION_INVALID_SYMBOL_EXCEPTION_HPP
