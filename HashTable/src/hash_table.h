#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "linked_hash_entry.h"
#include <string>

uint32_t jenkins_one_at_a_time_hash(std::string const& key);

template< typename Key >
uint32_t hash_generator(Key const&);

const uint32_t DEFAULT_TABLE_SIZE = 128;
const double DEFAULT_THRESHOLD = 0.7;

template< typename Key, typename Value >
    class HashTable {
    public:
        HashTable(uint32_t (*)(Key const&) = &hash_generator, uint32_t size_ = DEFAULT_TABLE_SIZE, double threshold_ = DEFAULT_THRESHOLD);
        HashTable(HashTable<Key, Value> const&);
        HashTable(HashTable<Key, Value>&&);
        HashTable& operator=(HashTable<Key, Value> const&);
        HashTable& operator=(HashTable<Key, Value>&&);
        ~HashTable();
        void set_threshold(double);
        uint32_t size() const;
        uint32_t max_size() const;
        bool empty() const;
        Value& at(Key const& key_);
        Value const& at(Key const& key_) const;
        Value& operator[](Key const&);
        void swap(HashTable<Key, Value>&);
        bool contains(const Key&) const;
        bool insert(Key const&, Value const&);
        bool erase(Key const&);
        double load_factor() const;
        double max_load_factor() const;
        void reserve(uint32_t count_);
        void rehash(uint32_t count_);
        void clear();
        bool operator==(HashTable<Key, Value> const&) const;
        bool operator!=(HashTable<Key, Value> const&) const;
    private:
        LinkedHashEntry<Key, Value> *_get_entry(Key const&, uint32_t hash_) const;
        uint32_t (*_hash_func)(Key const&);
        double _threshold_;
        uint32_t _max_size_;
        uint32_t _table_size_;
        uint32_t _size_;
        LinkedHashEntry<Key, Value> **_table_;
        LinkedHashEntry<Key, Value> **_copy_table() const;
    };

#include "hash_table.tpp"

#endif //HASH_TABLE_H