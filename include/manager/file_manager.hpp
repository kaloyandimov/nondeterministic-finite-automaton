#ifndef MANAGER_FILE_MANAGER_HPP
#define MANAGER_FILE_MANAGER_HPP

#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>

#include "storage/storage.hpp"
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

    void read(Storage<T>&) const;
    void read(Storage<T>&, const std::filesystem::path&) const;

    void save(const T&);
    void save(const Storage<T>&);

    void save_as(const T&, const std::filesystem::path&);
    void save_as(const Storage<T>&, const std::filesystem::path&);
    
 private:
    S serializer_;
    std::filesystem::path path_;

    void write(const T&, const std::filesystem::path&) const;
    void write(const Storage<T>&, const std::filesystem::path&) const;
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
void FileManager<T, S>::write(const T& object, const std::filesystem::path& path) const {
    std::ofstream file{path};
    
    file << 1 << '\n';

    serializer_.write(file, object);
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::write(const Storage<T>& storage, const std::filesystem::path& path) const {
    std::ofstream file{path};
    
    file << storage.size() << '\n';

    for (const auto& entry : storage) {
        serializer_.write(file, entry.second);
    }
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::save(const T& object) {
    save_as(object, path_);
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::save(const Storage<T>& storage) {
    save_as(storage, path_);
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::save_as(const T& object, const std::filesystem::path& path) {
    write(object, path);

    path_ = path;
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::save_as(const Storage<T>& storage, const std::filesystem::path& path) {
    write(storage, path);

    path_ = path;
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::read(Storage<T>& storage) const {
    read(storage, path_);
}

template <typename T, typename S>
requires SerializerFor<S, T>
void FileManager<T, S>::read(Storage<T>& storage, const std::filesystem::path& path) const {
    std::ifstream file{path};
    int count = 0;

    file >> count;

    for (int i = 0; i < count; i++) {
        storage.add(serializer_.read(file));
    }
}

#endif  // MANAGER_FILE_MANAGER_HPP
