#ifndef SERIALIZATION_SERIALIZER_FOR_HPP
#define SERIALIZATION_SERIALIZER_FOR_HPP

#include <concepts>
#include <istream>
#include <ostream>

template<typename S, typename T>
concept SerializerFor = requires(const S s, std::istream& in, std::ostream& out, const T& value) {
    { s.read(in) } -> std::same_as<T>;
    { s.write(out, value) } -> std::same_as<void>;
};

#endif // SERIALIZATION_SERIALIZER_FOR_HPP
