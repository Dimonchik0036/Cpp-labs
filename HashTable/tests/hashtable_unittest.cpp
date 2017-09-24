#include "gtest/gtest.h"

#include "hash_table.h"

TEST(ConstructorHashTable_Test, DefaultHashFunc) {
    HashTable<double, int> hash_table_;
}

TEST(ConstructorHashTable_Test, CustomHashFunc) {
    HashTable<double, int> hash_table_(hash_generator);
}

TEST(ConstructorHashTable_Test, CopyConstructor) {
    HashTable<double, int> hash_table_1_;
    HashTable<double, int> hash_table_2_(hash_table_1_);
}

TEST(ConstructorHashTable_Test, AssignmentConstructor) {
    HashTable<double, int> hash_table_1_;
    hash_table_1_.insert(5.1, 10);

    HashTable<double, int> hash_table_2_;
    hash_table_2_ = hash_table_1_;

    EXPECT_EQ(hash_table_2_, hash_table_1_);
}

TEST(ConstructorHashTable_Test, DifferentKeyAndValue) {
    HashTable<int, int> hash_table_1_;
    HashTable<bool, int> hash_table_2_;
    HashTable<std::string, int> hash_table_3_;
    HashTable<double, float> hash_table_5_;
    HashTable<std::string, bool> hash_table_6_;

    HashTable<std::vector<int>, std::string> hash_table_7_(
            [](std::vector<int> const& vector_) -> uint32_t {return static_cast<uint32_t >(vector_.size() + 0);});
}

TEST(MethodHashTable_Test, DifferentInserts) {
    HashTable<std::string, int> hash_table_;
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_TRUE(hash_table_.insert("Second key", 2));
    EXPECT_EQ(hash_table_.size(), 2);

    EXPECT_TRUE(hash_table_.insert("Third key", 1));
    EXPECT_TRUE(hash_table_.insert("Fourth key", 2));
    EXPECT_EQ(hash_table_.size(), 4);
}

TEST(MethodHashTable_Test, Empty) {
    HashTable<std::string, int> hash_table_;
    EXPECT_TRUE(hash_table_.empty());
}

TEST(MethodHashTable_Test, SameInserts) {
    HashTable<std::string, int> hash_table_;
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_TRUE(hash_table_.insert("First key", 10));
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_TRUE(hash_table_.insert("First key", 100));
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_TRUE(hash_table_.insert("First key", 20));
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_TRUE(hash_table_.insert("Second key", 2));
    EXPECT_EQ(hash_table_.size(), 2);
}

TEST(MethodHashTable_Test, InsertsAndGet) {
    HashTable<std::string, int> hash_table_;
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_EQ(hash_table_.at("First key"), 1);
    EXPECT_TRUE(hash_table_.insert("First key", 10));
    EXPECT_EQ(hash_table_.at("First key"), 10);
    EXPECT_TRUE(hash_table_.insert("First key", 100));
    EXPECT_EQ(hash_table_.at("First key"), 100);
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_EQ(hash_table_.at("First key"), 1);
    EXPECT_TRUE(hash_table_.insert("First key", 20));
    EXPECT_EQ(hash_table_.at("First key"), 20);
    EXPECT_TRUE(hash_table_.insert("First key", 1));
    EXPECT_EQ(hash_table_.at("First key"), 1);
    EXPECT_TRUE(hash_table_.insert("Second key", 2));
    EXPECT_EQ(hash_table_.at("Second key"), 2);
}

TEST(MethodHashTable_Test, At) {
    HashTable<std::string, int> hash_table_;
    EXPECT_THROW(hash_table_.at("Test"), std::invalid_argument);
}