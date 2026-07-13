#ifndef MANAGER_FILE_MANAGER_HPP
#define MANAGER_FILE_MANAGER_HPP

#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>

#include "serialization/serializer_for.hpp"

template <typename T, typename S>
requires SerializerFor<S, T>
class FileManager {
 public:
    explicit FileManager(S serializer = S{});

    bool is_open() const;
    bool file_exists() const;

    void open(const std::filesystem::path&);
    void close();

    std::vector<T> load() const;

    void save(const T&);
    void save(const std::vector<T>&);

    void save_as(const T&, const std::filesystem::path&);
    void save_as(const std::vector<T>&, const std::filesystem::path&);
    
 private:
    S serializer_;
    std::filesystem::path path_;

    void write(const T&, const std::filesystem::path&) const;
    void write(const std::vector<T>&, const std::filesystem::path&) const;
};

template <typename T, typename S>
requires SerializerFor<S, T>
FileManager<T, S>::FileManager(S serializer) : serializer_(std::move(serializer)) {}

template <typename T, typename S>
requires SerializerFor<S, T>
bool FileManager<T, S>::is_open() const {
    return !path_.empty();
}

template <typename T, typename S>
requires SerializerFor<S, T>
bool FileManager<T, S>::file_exists() const {
    return std::filesystem::exists(path_);
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::open(const std::filesystem::path& path) {
    path_ = path;
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::close() {
    path_.clear();
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::write(const T& value, const std::filesystem::path& path) const {
    std::ofstream file{path_};
    
    file << 1 << '\n';

    serializer_.write(file, value);
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::write(const std::vector<T>& values, const std::filesystem::path& path) const {
    std::ofstream file{path_};
    
    file << values.size() << '\n';

    for (const auto& value : values) {
        serializer_.write(file, value);
    }
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::save(const T& value) {
    save_as(value, path_);
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::save(const std::vector<T>& values) {
    save_as(values, path_);
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::save_as(const T& value, const std::filesystem::path& path) {
    write(value, path);

    path_ = path;
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::save_as(const std::vector<T>& values, const std::filesystem::path& path) {
    write(values, path);

    path_ = path;
}

template <typename T, typename S>
requires SerializerFor<S, T>
std::vector<T> FileManager<T, S>::load() const {
    std::ifstream file{path_};
    std::vector<T> values;
    int count = 0;

    file >> count;

    for (int i = 0; i < count; i++) {
        values.push_back(serializer_.read(file));
    }

    return values;
}

#endif  // MANAGER_FILE_MANAGER_HPP
