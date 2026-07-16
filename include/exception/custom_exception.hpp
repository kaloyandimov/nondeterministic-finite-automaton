#ifndef EXCEPTION_CUSTOM_EXCEPTION_HPP
#define EXCEPTION_CUSTOM_EXCEPTION_HPP

#include <stdexcept>

class CustomException : public std::logic_error {
 public:
    using std::logic_error::logic_error;
};

#endif // EXCEPTION_CUSTOM_EXCEPTION_HPP
