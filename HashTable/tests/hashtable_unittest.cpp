#include "gtest/gtest.h"

#include "hash_table.h"

TEST(HashTable_Constructor, Default) {
    HashTable<double, int> hash_table1_;
    HashTable<std::string, int> hash_table2_;

    typedef std::string Key;
    struct Value {
        unsigned age;
        unsigned weight;
    };

    HashTable<Key, Value> hash_table3_;
}

TEST(HashTable_Constructor, CustomHash) {
    HashTable<double, int> hash_table_(hash_generator);
    HashTable<std::vector<int>, std::string> hash_table_7_(
            [](std::vector<int> const& vector_) -> uint32_t {return static_cast<uint32_t >(vector_.size() + 0);});
}

TEST(HashTable_Constructor, CustomSize) {
    HashTable<std::string, int> hash_table_(hash_generator, 10, 0.7);
}

TEST(HashTable_Constructor, Assignment) {
    HashTable<double, int> hash_table_1_;
    hash_table_1_.insert(5.1, 10);

    HashTable<double, int> hash_table_2_;
    hash_table_2_ = hash_table_1_;

    EXPECT_EQ(hash_table_2_, hash_table_1_);
}

TEST(HashTable_SetThreshold, Default) {
    HashTable<std::string, int> hash_table_;
    double threshold_ = 0.5;

    hash_table_.set_threshold(threshold_);

    EXPECT_DOUBLE_EQ(threshold_, hash_table_.max_load_factor());
}

TEST(HashTable_SetThreshold, Zero) {
    HashTable<std::string, int> hash_table_;
    double threshold_ = 0;

    hash_table_.set_threshold(threshold_);

    EXPECT_DOUBLE_EQ(threshold_, hash_table_.max_load_factor());
}

TEST(HashTable_Size, Zero) {
    HashTable<std::string, int> hash_table_;

    EXPECT_EQ(hash_table_.size(), 0);
}

TEST(HashTable_Size, Several) {
    HashTable<int, int> hash_table_;
    for (int i_ = 0; i_ < 100; ++i_) {
        hash_table_.insert(i_, i_);
    }

    EXPECT_EQ(hash_table_.size(), 100);
}

TEST(HashTable_Insert, Single) {
    HashTable<std::string, int> hash_table_;

    EXPECT_TRUE(hash_table_.insert("Single", 1));

    EXPECT_EQ(hash_table_.size(), 1);
}

TEST(HashTable_Insert, Several) {
    HashTable<std::string, int> hash_table_;
    hash_table_.insert("One", 1);
    hash_table_.insert("Two", 1);

    EXPECT_EQ(hash_table_.size(), 2);
}

TEST(HashTable_Insert, Equal) {
    HashTable<std::string, int> hash_table_;
    int const old_val_ = 1;

    hash_table_.insert("One", old_val_);
    int val1_ = hash_table_.at("One");

    hash_table_.insert("One", old_val_ + 1);
    int val2_ = hash_table_.at("One");

    EXPECT_EQ(old_val_, val1_);
    EXPECT_NE(old_val_, val2_);
}

TEST(HashTable_Erase, Single) {
    HashTable<std::string, int> hash_table_;
    hash_table_.insert("One", 1);
    hash_table_.erase("One");

    EXPECT_EQ(hash_table_.size(), 0);
}

TEST(HashTable_Empty, Empty) {
    HashTable<std::string, int> hash_table_;

    bool result_ = hash_table_.empty();

    EXPECT_TRUE(result_);
}

TEST(HashTable_Empty, NotEmpty) {
    HashTable<std::string, int> hash_table_;
    hash_table_.insert("Key", 0);

    bool result_ = hash_table_.empty();

    EXPECT_FALSE(result_);
}

TEST(HashTable_At, NotFound) {
    HashTable<std::string, int> hash_table_;

    EXPECT_THROW(hash_table_.at("Test"), std::invalid_argument);
}

TEST(HashTable_At, Found) {
    HashTable<std::string, int> hash_table_;
    std::string const key_ = "Key";
    int const val_ = 1;
    hash_table_.insert(key_, val_);

    int result_ = hash_table_.at(key_);

    EXPECT_EQ(result_, val_);
}

TEST(HashTable_At, ChangeVal) {
    HashTable<std::string, int> hash_table_;
    std::string const key_ = "Key";
    int const val1_ = 1;
    int const val2_ = 2;
    hash_table_.insert(key_, val1_);

    hash_table_.at(key_) = val2_;
    int const result_ = hash_table_.at(key_);

    EXPECT_EQ(result_, val2_);
}

TEST(HashTable_Indexing, Found) {
    HashTable<std::string, int> hash_table_;
    std::string const key_ = "Key";
    int const val_ = 1;
    hash_table_.insert(key_, val_);

    int const result_ = hash_table_[key_];

    EXPECT_EQ(result_, val_);
}

TEST(HashTable_Indexing, NotChangeSize) {
    HashTable<std::string, int> hash_table_;
    std::string const key_ = "Key";
    int const val_ = 1;
    hash_table_.insert(key_, val_);

    hash_table_[key_];

    EXPECT_EQ(hash_table_.size(), 1);
}

TEST(HashTable_Indexing, ChangeSize) {
    HashTable<std::string, int> hash_table_;
    std::string const key_ = "Key";

    hash_table_[key_];

    EXPECT_EQ(hash_table_.size(), 1);
}

TEST(HashTable_Indexing, DefaultConstructor) {
    HashTable<int, std::string> hash_table_;
    int const key_ = 1;

    hash_table_[key_];

    EXPECT_EQ(hash_table_.at(key_), std::string());
}

TEST(HashTable_Indexing, ChangeVal) {
    HashTable<std::string, int> hash_table_;
    std::string const key_ = "Key";
    int const val1_ = 1;
    int const val2_ = 2;
    hash_table_[key_] = val1_;

    hash_table_[key_] = val2_;
    int const result_ = hash_table_.at(key_);

    EXPECT_EQ(result_, val2_);
}

TEST(HashTable_Contains, NotContain) {
    HashTable<std::string, int> hash_table_;

    bool result_ = hash_table_.contains("Key");

    EXPECT_FALSE(result_);
}

TEST(HashTable_Contains, Contain) {
    HashTable<std::string, int> hash_table_;
    std::string const key_ = "Key";
    hash_table_.insert(key_, 0);

    bool result_ = hash_table_.contains(key_);

    EXPECT_TRUE(result_);
}

TEST(HashTable_LoadFactor, Increment) {
    HashTable<std::string, int> hash_table_;

    double const old_factor_ = hash_table_.load_factor();
    hash_table_.insert("Key", 10);
    double const factor_ = hash_table_.load_factor();

    EXPECT_LE(old_factor_, factor_);
}

TEST(HashTable_MaxLoadFactor, Check) {
    HashTable<std::string, int> hash_table_;

    double const old_factor_ = hash_table_.max_load_factor();
    hash_table_.set_threshold(0.5);
    double const factor_ = hash_table_.max_load_factor();

    EXPECT_NE(old_factor_, factor_);
}

TEST(HashTable_Reserve, Check) {
    HashTable<std::string, int> hash_table_;

    uint32_t const old_max_size_ = hash_table_.max_size();
    hash_table_.reserve(1000);
    uint32_t const new_max_size_ = hash_table_.max_size();

    EXPECT_LE(old_max_size_, new_max_size_);
}

TEST(HashTable_Rehash, Check) {
    HashTable<std::string, int> hash_table_;

    uint32_t const old_max_size_ = hash_table_.max_size();
    hash_table_.rehash(1000);
    uint32_t const new_max_size_ = hash_table_.max_size();

    EXPECT_LE(old_max_size_, new_max_size_);
}

TEST(HashTable_Clear, Empty) {
    HashTable<std::string, int> hash_table_;

    hash_table_.clear();

    EXPECT_EQ(hash_table_.size(), 0);
}

TEST(HashTable_Clear, Full) {
    HashTable<std::string, int> hash_table_;
    hash_table_.insert("Two", 2);

    hash_table_.clear();

    EXPECT_EQ(hash_table_.size(), 0);
}

TEST(HashTable_Swap, Equal) {
    HashTable<std::string, int> hash_table_;

    hash_table_.swap(hash_table_);
}

TEST(HashTable_Swap, NotEqual) {
    HashTable<int, int> hash_table1_;
    hash_table1_.insert(1, 1);
    hash_table1_.insert(2, 2);
    hash_table1_.insert(3, 3);
    const HashTable<int, int> hash_table1_copy_ = hash_table1_;

    HashTable<int, int> hash_table2_;
    hash_table2_.insert(2, 2);
    hash_table2_.insert(3, 3);
    hash_table2_.insert(4, 4);
    const HashTable<int, int> hash_table2_copy_ = hash_table2_;

    hash_table1_.swap(hash_table2_);

    EXPECT_EQ(hash_table1_copy_, hash_table2_);
    EXPECT_EQ(hash_table2_copy_, hash_table1_);
}

TEST(HashTable_Equality, Equal) {
    HashTable<int, int> hash_table1_;
    hash_table1_.insert(1, 1);
    hash_table1_.insert(2, 2);
    hash_table1_.insert(3, 3);

    HashTable<int, int> hash_table2_;
    hash_table2_.insert(2, 2);
    hash_table2_.insert(3, 3);
    hash_table2_.insert(1, 1);

    bool result_ = hash_table1_ == hash_table2_;

    EXPECT_TRUE(result_);
}

TEST(HashTable_Equality, NotEqual) {
    HashTable<int, int> hash_table1_;
    hash_table1_.insert(1, 1);
    hash_table1_.insert(2, 1);
    hash_table1_.insert(3, 3);

    HashTable<int, int> hash_table2_;
    hash_table2_.insert(2, 2);
    hash_table2_.insert(3, 3);
    hash_table2_.insert(1, 1);

    bool result_ = hash_table1_ != hash_table2_;

    EXPECT_TRUE(result_);
}
