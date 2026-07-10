#include "exception/invalid_expression_exception.hpp"

InvalidExpressionException::InvalidExpressionException(const std::string& what_arg) : CustomException{what_arg} {}
