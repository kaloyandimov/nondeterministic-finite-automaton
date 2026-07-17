#ifndef STORAGE_STORAGE_HPP
#define STORAGE_STORAGE_HPP

#include <cstddef>
#include <map>
#include <utility>

template <typename T>
class Storage {
 public:
    using Id = std::size_t;
    using Container = std::map<Id, T>;
    using SizeType = typename Container::size_type;
    using ConstIterator = typename Container::const_iterator;

    bool empty() const noexcept;
    SizeType size() const noexcept;
    bool contains(Id id) const noexcept;

    T& get(Id id);
    const T& get(Id id) const;

    Id add(T object);
    bool remove(Id id);
    void clear() noexcept;

    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;

 private:
    Container objects_;
    Id next_id_{1};
};

template <typename T>
bool Storage<T>::empty() const noexcept {
    return objects_.empty();
}

template <typename T>
typename Storage<T>::SizeType Storage<T>::size() const noexcept {
    return objects_.size();
}

template <typename T>
bool Storage<T>::contains(Id id) const noexcept {
    return objects_.contains(id);
}

template <typename T>
T& Storage<T>::get(Id id) {
    return objects_.at(id);
}

template <typename T>
const T& Storage<T>::get(Id id) const {
    return objects_.at(id);
}

template <typename T>
typename Storage<T>::Id Storage<T>::add(T object) {
    const Id id{next_id_++};

    objects_.emplace(id, std::move(object));

    return id;
}

template <typename T>
bool Storage<T>::remove(Storage::Id id) {
    return objects_.erase(id) == 1;
}

template <typename T>
void Storage<T>::clear() noexcept {
    objects_.clear();
    next_id_ = 1;
}

template <typename T>
typename Storage<T>::ConstIterator Storage<T>::begin() const noexcept {
    return objects_.begin();
}

template <typename T>
typename Storage<T>::ConstIterator Storage<T>::end() const noexcept {
    return objects_.end();
}

#endif // STORAGE_STORAGE_HPP
