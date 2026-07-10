#include "exception/invalid_symbol_exception.hpp"

InvalidSymbolException::InvalidSymbolException(const std::string& what_arg) : CustomException{what_arg} {}
