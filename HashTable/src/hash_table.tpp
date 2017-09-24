#include <stdexcept>
#include <cstring>
#include "hash_table.h"

template< typename Key >
uint32_t hash_generator(Key const& key_) {
    std::hash<Key> f;
    return static_cast<uint32_t>(f(key_));
}

template< typename Key, typename Value >
void HashTable<Key, Value>::resize() {
    uint32_t old_table_size_ = _table_size_;
    _table_size_ *= 2;
    _max_size_ = (uint32_t)(_table_size_ * _threshold_);
    LinkedHashEntry<Key, Value> **old_table_ = _table_;

    _table_ = new LinkedHashEntry<Key, Value> *[_table_size_];
    memset(_table_, 0, _table_size_ * sizeof(LinkedHashEntry<Key, Value> *));
    _size_ = 0;

    for (auto hash_ = 0; hash_ < old_table_size_; ++hash_) {
        if (old_table_[hash_]) {
            LinkedHashEntry<Key, Value> *old_entry_;
            LinkedHashEntry<Key, Value> *entry_ = old_table_[hash_];
            while (entry_) {
                insert(entry_->get_key(), entry_->get_value());
                old_entry_ = entry_;
                entry_ = entry_->get_next();
                delete old_entry_;
            }
        }
    }

    delete[] old_table_;
}

template< typename Key, typename Value >
HashTable<Key, Value>::HashTable(uint32_t (*hash_func)(Key const&), uint32_t size_, double threshold_)
        : _threshold_(threshold_),
          _max_size_(static_cast<uint32_t>(size_ * threshold_)),
          _table_size_(size_),
          _size_(0),
          _hash_func(hash_func) {
    _table_ = new LinkedHashEntry<Key, Value> *[_table_size_];
    memset(_table_, 0, _table_size_ * sizeof(LinkedHashEntry<Key, Value> *));
}

template< typename Key, typename Value >
void HashTable<Key, Value>::set_threshold(double threshold_) {
    _threshold_ = threshold_;
    _max_size_ = (uint32_t)(_table_size_ * threshold_);
}
template< typename Key, typename Value >
LinkedHashEntry<Key, Value> *HashTable<Key, Value>::_get_entry(Key const& key_, uint32_t hash_) const {
    if (!_table_[hash_]) {
        return nullptr;
    }

    LinkedHashEntry<Key, Value> *entry_ = _table_[hash_];
    while (entry_->get_key() != key_ && entry_->get_next()) {
        entry_ = entry_->get_next();
    }

    return entry_;

};

template< typename Key, typename Value >
Value& HashTable<Key, Value>::operator[](const Key& key_) {
    uint32_t hash_ = _hash_func(key_) % _table_size_;

    LinkedHashEntry<Key, Value> *entry_ = _get_entry(key_, hash_);
    if (!entry_) {
        entry_ = new LinkedHashEntry<Key, Value>(key_, Value());
        _table_[hash_] = entry_;
        ++_size_;
        return entry_->get_value();
    }

    if (entry_->get_key() != key_) {
        LinkedHashEntry<Key, Value> *next_entry_ = new LinkedHashEntry<Key, Value>(key_, Value());
        entry_->set_next(next_entry_);
        ++_size_;
        return next_entry_->get_value();
    }

    return entry_->get_value();
}

template< typename Key, typename Value >
Value& HashTable<Key, Value>::at(Key const& key_) {
    uint32_t hash_ = _hash_func(key_) % _table_size_;
    LinkedHashEntry<Key, Value> *entry_ = _get_entry(key_, hash_);

    if (!entry_ || entry_->get_key() != key_) {
        throw std::invalid_argument("Key not found");
    }

    return entry_->get_value();
}

template< typename Key, typename Value >
Value const& HashTable<Key, Value>::at(Key const& key_) const {
    uint32_t hash_ = _hash_func(key_) % _table_size_;
    LinkedHashEntry<Key, Value> *entry_ = _get_entry(key_, hash_);

    if (!entry_ || entry_->get_key() != key_) {
        throw std::invalid_argument("Key not found");
    }

    return entry_->get_value();
}

template< typename Key, typename Value >
bool HashTable<Key, Value>::insert(Key const& key_, Value const& value_) {
    uint32_t hash_ = _hash_func(key_) % _table_size_;
    LinkedHashEntry<Key, Value> *entry_ = _get_entry(key_, hash_);
    if (!entry_) {
        _table_[hash_] = new LinkedHashEntry<Key, Value>(key_, value_);
        ++_size_;
        return true;
    }

    if (entry_->get_key() == key_) {
        entry_->set_value(value_);
        return true;
    }

    entry_->set_next(new LinkedHashEntry<Key, Value>(key_, value_));
    ++_size_;

    if (_size_ >= _max_size_) {
        resize();
    }

    return true;
}


template< typename Key, typename Value >
bool HashTable<Key, Value>::erase(Key const& key_) {
    uint32_t hash_ = _hash_func(key_) % _table_size_;
    if (!_table_[hash_]) {
        return false;
    }

    LinkedHashEntry<Key, Value> *prev_entry_ = nullptr;
    LinkedHashEntry<Key, Value> *entry_ = _table_[hash_];

    while (entry_->get_next() && entry_->get_key() != key_) {
        prev_entry_ = entry_;
        entry_ = entry_->get_next();
    }

    if (entry_->get_key() != key_) {
        return false;
    }

    if (!prev_entry_) {
        LinkedHashEntry<Key, Value> *next_entry_ = entry_->get_next();
        delete entry_;
        _table_[hash_] = next_entry_;
    } else {
        LinkedHashEntry<Key, Value> *next_ = entry_->get_next();
        delete entry_;
        prev_entry_->set_next(next_);
    }

    --_size_;
    return true;
}


template< typename Key, typename Value >
HashTable<Key, Value>::~HashTable<Key, Value>() {
    clear();
    delete[] _table_;
}

template< typename Key, typename Value >
size_t HashTable<Key, Value>::size() const {
    return _size_;
}

template< typename Key, typename Value >
bool HashTable<Key, Value>::empty() const {
    return _size_ == 0;
}

template< typename Key, typename Value >
bool HashTable<Key, Value>::contains(const Key& key_) const {
    try {
        at(key_);
        return true;
    } catch (std::invalid_argument) {
        return false;
    }
}

template< typename Key, typename Value >
void HashTable<Key, Value>::clear() {
    if (!_table_) {
        return;
    }

    for (auto hash_ = 0; hash_ < _table_size_; ++hash_) {
        if (_table_[hash_]) {
            LinkedHashEntry<Key, Value> *prev_entry_ = nullptr;
            LinkedHashEntry<Key, Value> *entry_ = _table_[hash_];

            while (entry_) {
                prev_entry_ = entry_;
                entry_ = entry_->get_next();
                delete prev_entry_;
            }
        }
    }

    _size_ = 0;
}

template< typename Key, typename Value >
void HashTable<Key, Value>::swap(HashTable<Key, Value>& other_) {
    if (this == &other_) {
        return;
    }

    std::swap(_size_, other_._size_);
    std::swap(_table_size_, other_._table_size_);
    std::swap(_table_, other_._table_);
    std::swap(_max_size_, other_._max_size_);
    std::swap(_threshold_, other_._threshold_);
    std::swap(_hash_func, other_._hash_func);
}

template< typename Key, typename Value >
HashTable<Key, Value>& HashTable<Key, Value>::operator=(HashTable<Key, Value> const& other_) {
    if (this == &other_) {
        return *this;
    }

    this->~HashTable();
    _size_ = other_._size_;
    _table_size_ = other_._table_size_;
    _max_size_ = other_._max_size_;
    _threshold_ = other_._threshold_;
    _hash_func = other_._hash_func;

    _table_ = other_._copy_table();

    return *this;
}

template< typename Key, typename Value >
HashTable<Key, Value>& HashTable<Key, Value>::operator=(HashTable<Key, Value>&& tmp_) {
    if (this == &tmp_) {
        return *this;
    }

    this->~HashTable();
    _size_ = tmp_._size_;
    _table_size_ = tmp_._table_size_;
    _max_size_ = tmp_._max_size_;
    _threshold_ = tmp_._threshold_;
    _hash_func = tmp_._hash_func;
    _table_ = tmp_._table_;

    tmp_._table_ = nullptr;
    return *this;
}

template< typename Key, typename Value >
LinkedHashEntry<Key, Value> **HashTable<Key, Value>::_copy_table() const {
    LinkedHashEntry<Key, Value> **new_table_ = new LinkedHashEntry<Key, Value> *[_table_size_];

    for (auto hash_ = 0; hash_ < _table_size_; ++hash_) {
        if (!_table_[hash_]) {
            new_table_[hash_] = nullptr;
            continue;
        }

        LinkedHashEntry<Key, Value> *entry_ = _table_[hash_];
        LinkedHashEntry<Key, Value> *new_entry_ = entry_->copy();
        new_table_[hash_] = new_entry_;

        while ((entry_ = entry_->get_next())) {
            new_entry_ = new_entry_->set_next(entry_->copy());
        }
    }

    return new_table_;
}

template< typename Key, typename Value >
HashTable<Key, Value>::HashTable(
        HashTable<Key, Value> const& other_)
        : _size_(other_._size_),
          _table_size_(other_._table_size_),
          _max_size_(other_._max_size_),
          _threshold_(other_._threshold_),
          _hash_func(other_._hash_func),
          _table_(other_._copy_table()) {}

template< typename Key, typename Value >
HashTable<Key, Value>::HashTable(
        HashTable<Key, Value>&& tmp_)
        : _size_(tmp_._size_),
          _table_size_(tmp_._table_size_),
          _max_size_(tmp_._max_size_),
          _threshold_(tmp_._threshold_),
          _hash_func(tmp_._hash_func),
          _table_(tmp_._table_) {
    tmp_._table_ = nullptr;
}

template< typename Key, typename Value >
bool HashTable<Key, Value>::operator==(HashTable<Key, Value> const& rhs_) const {
    if (_size_ != rhs_._size_ || _table_size_ != rhs_._table_size_) {
        return false;
    }

    for (auto hash_ = 0; hash_ < _table_size_; ++hash_) {
        if (!_table_[hash_]) {
            continue;
        }

        LinkedHashEntry<Key, Value> *entry_ = _table_[hash_];
        do {
            if (!rhs_.contains(entry_->get_key())) {
                return false;
            }

        } while ((entry_ = entry_->get_next()));
    }

    return true;
}


template< typename Key, typename Value >
bool HashTable<Key, Value>::operator!=(HashTable<Key, Value> const& rhs_) const {
    return !(*this == rhs_);
}