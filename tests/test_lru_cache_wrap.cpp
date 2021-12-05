#include <gtest/gtest.h>
#include "cpp_cachetools/cpp_cache_tools.hpp"


class LRUCacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        LRUCacheTest::calls_per_value.clear();
    }

    static std::unordered_map<int, int> calls_per_value;

    static int test_function(int value) {
        auto&& called_iter = calls_per_value.find(value);
        if(called_iter == calls_per_value.end()) {
            calls_per_value.emplace(value, 1);
        } else {
            auto &&number_of_calls = called_iter->second;
            calls_per_value.insert_or_assign(called_iter, value, number_of_calls + 1);
        }
        return value * 2;
    }
};

std::unordered_map<int, int> LRUCacheTest::calls_per_value{};


TEST_F(LRUCacheTest, CallsOnce) {
    auto &&wrapped = LRUCache::wrap(LRUCacheTest::test_function, 2);
    int cached_calls_number = 5;
    for(auto i = 0; i < cached_calls_number; ++i)
        EXPECT_EQ(wrapped(1), 2);
    auto &&actually_called_number = calls_per_value.find(1)->second;
    EXPECT_EQ(actually_called_number, 1);
}

TEST_F(LRUCacheTest, CallsOnceOnMultipleArguments) {
    auto &&wrapped = LRUCache::wrap(LRUCacheTest::test_function, 2);
    int cached_calls_number = 5;
    for(auto i = 0; i < cached_calls_number; ++i) {
        EXPECT_EQ(wrapped(1), 2);
        EXPECT_EQ(wrapped(2), 4);
    }
    auto &&actually_called_number_first = calls_per_value.find(1)->second;
    auto &&actually_called_number_second = calls_per_value.find(2)->second;
    EXPECT_EQ(actually_called_number_first, 1);
    EXPECT_EQ(actually_called_number_second, 1);
}

TEST_F(LRUCacheTest, CallsMultipleTimesWhenEvicting) {
    auto &&wrapped = LRUCache::wrap(LRUCacheTest::test_function, 1);
    int cached_calls_number = 5;
    for(auto i = 0; i < cached_calls_number; ++i) {
        ASSERT_EQ(wrapped(1), 2);
        ASSERT_EQ(wrapped(2), 4);
    }
    auto &&actually_called_number_first = calls_per_value.find(1)->second;
    auto &&actually_called_number_second = calls_per_value.find(2)->second;
    EXPECT_EQ(actually_called_number_first, cached_calls_number);
    EXPECT_EQ(actually_called_number_second, cached_calls_number);
}


TEST_F(LRUCacheTest, EvictsLeastRecentlyUsed) {
    auto &&wrapped = LRUCache::wrap(LRUCacheTest::test_function, 2);
    int cached_calls_number = 5;
    ASSERT_EQ(wrapped(1), 2);
    ASSERT_EQ(wrapped(2), 4);
    ASSERT_EQ(calls_per_value.find(1)->second, 1);
    ASSERT_EQ(wrapped(3), 6);
    ASSERT_EQ(wrapped(2), 4);
    ASSERT_EQ(calls_per_value.find(2)->second, 1);
    ASSERT_EQ(wrapped(1), 2);
    ASSERT_EQ(calls_per_value.find(1)->second, 2);
}