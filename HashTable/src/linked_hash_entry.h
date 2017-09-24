#ifndef LINKED_HASH_ENTRY_H
#define LINKED_HASH_ENTRY_H

template< typename Key, typename Value >
    class LinkedHashEntry {
    public:
        LinkedHashEntry(Key const&, Value const&);
        Key get_key() const;
        Value get_value() const;
        Value& get_value();
        LinkedHashEntry *get_next() const;
        void set_value(Value const&);
        LinkedHashEntry *set_next(LinkedHashEntry *);
        LinkedHashEntry *copy() const;
        bool operator==(LinkedHashEntry<Key, Value> const&) const;
        bool operator!=(LinkedHashEntry<Key, Value> const&) const;
    private:
        Key _key_;
        Value _value_;
        LinkedHashEntry *_next_;
    };

#include "linked_hash_entry.tpp"

#endif //LINKED_HASH_ENTRY_H
