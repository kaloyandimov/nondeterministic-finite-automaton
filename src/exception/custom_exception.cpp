#include "exception/custom_exception.hpp"

CustomException::CustomException(const std::string& what_arg) : std::logic_error{what_arg} {}

char const* CustomException::what() const noexcept {
    return std::logic_error::what();
}
