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
    using ConstIterator = Container::const_iterator;

    bool contains(Id) const;

    Container::size_type size() const;

    T& get(Id);
    const T& get(Id) const;

    Id add(T);

    void remove(Id);

    ConstIterator begin() const;
    ConstIterator end() const;

 private:
    Container objects_;
    Id next_id_{1};
};

template <typename T>
bool Storage<T>::contains(Storage::Id id) const {
    return objects_.contains(id);
}

template <typename T>
Storage<T>::Container::size_type Storage<T>::size() const {
    return objects_.size();
}

template <typename T>
T& Storage<T>::get(Storage::Id id) {
    return objects_.at(id);
}

template <typename T>
const T& Storage<T>::get(Storage::Id id) const {
    return objects_.at(id);
}

template <typename T>
Storage<T>::Id Storage<T>::add(T object) {
    const Storage::Id id = next_id_++;

    objects_.emplace(id, std::move(object));

    return id;
}

template <typename T>
void Storage<T>::remove(Storage::Id id) {
    objects_.erase(id);
}

template <typename T>
Storage<T>::ConstIterator Storage<T>::begin() const {
    return objects_.begin();
}

template <typename T>
Storage<T>::ConstIterator Storage<T>::end() const {
    return objects_.end();
}

#endif // STORAGE_STORAGE_HPP
