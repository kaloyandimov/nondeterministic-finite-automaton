#ifndef EXCEPTION_UNBALANCED_BRACKETS_EXCEPTION_HPP
#define EXCEPTION_UNBALANCED_BRACKETS_EXCEPTION_HPP

#include "exception/custom_exception.hpp"

class UnbalancedBracketsException : public CustomException {
 public:
    using CustomException::CustomException;
};

#endif // EXCEPTION_UNBALANCED_BRACKETS_EXCEPTION_HPP
