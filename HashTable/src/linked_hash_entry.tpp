#include "linked_hash_entry.h"

template< typename Key, typename Value >
LinkedHashEntry<Key, Value>::LinkedHashEntry(Key const& key_, Value const& value_)
        : _key_(key_),
          _value_(value_),
          _next_(nullptr) {}

template< typename Key, typename Value >
Key LinkedHashEntry<Key, Value>::get_key() const {
    return _key_;
}

template< typename Key, typename Value >
Value LinkedHashEntry<Key, Value>::get_value() const {
    return _value_;
}

template< typename Key, typename Value >
Value& LinkedHashEntry<Key, Value>::get_value() {
    return _value_;
}

template< typename Key, typename Value >
LinkedHashEntry<Key, Value> *LinkedHashEntry<Key, Value>::get_next() const {
    return _next_;
}

template< typename Key, typename Value >
void LinkedHashEntry<Key, Value>::set_value(Value const& value_) {
    _value_ = value_;
}

template< typename Key, typename Value >
LinkedHashEntry<Key, Value> *LinkedHashEntry<Key, Value>::set_next(LinkedHashEntry<Key, Value> *next_) {
    _next_ = next_;
    return _next_;
}

template< typename Key, typename Value >
LinkedHashEntry<Key, Value> *LinkedHashEntry<Key, Value>::copy() const {
    return new LinkedHashEntry<Key, Value>(_key_, _value_);
}

template< typename Key, typename Value >
bool LinkedHashEntry<Key, Value>::operator==(LinkedHashEntry<Key, Value> const& rhs_) const {
    return _value_ == rhs_._value_ && _key_ == rhs_._key_;
}

template< typename Key, typename Value >
bool LinkedHashEntry<Key, Value>::operator!=(LinkedHashEntry<Key, Value> const& rhs_) const {
    return !(*this == rhs_);
}
