#include "exception/invalid_argument_count_exception.hpp"

InvalidArgumentCountException::InvalidArgumentCountException(const std::string& what_arg) : CustomException{what_arg} {}
