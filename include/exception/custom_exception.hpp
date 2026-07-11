#ifndef EXCEPTION_CUSTOM_EXCEPTION_HPP
#define EXCEPTION_CUSTOM_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class CustomException : public std::logic_error {
   public:
    CustomException(const std::string& what_arg);

    char const* what() const noexcept override;
};

#endif /* EXCEPTION_CUSTOM_EXCEPTION_HPP */
