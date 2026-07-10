#ifndef EXCEPTION_INVALID_SYMBOL_EXCEPTION_HPP
#define EXCEPTION_INVALID_SYMBOL_EXCEPTION_HPP

#include "exception/custom_exception.hpp"

class InvalidSymbolException : public CustomException {
   public:
    InvalidSymbolException(const std::string& what_arg);
};

#endif /* EXCEPTION_INVALID_SYMBOL_EXCEPTION_HPP */
