#include "exception/unbalanced_brackets_exception.hpp"

UnbalancedBracketsException::UnbalancedBracketsException(const std::string& what_arg) : CustomException{what_arg} {}
