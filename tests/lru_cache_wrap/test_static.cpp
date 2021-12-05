#include <gtest/gtest.h>
#include "cpp_cachetools/cpp_cache_tools.hpp"
#include "tests/counter.h"


class LRUCacheTest : public ::testing::Test {
protected:
    void SetUp() override {
        counter = std::make_shared<MyCounter>();
    }
    static int test_function(int value) {
        counter->increment(value);
        return value * 2;
    }
    static std::shared_ptr<MyCounter> counter;
};

std::shared_ptr<MyCounter> LRUCacheTest::counter{};

TEST_F(LRUCacheTest, CallsOnce) {
    auto &&wrapped = LRUCache::wrap(LRUCacheTest::test_function, 2);
    int cached_calls_number = 5;
    for(auto i = 0; i < cached_calls_number; ++i)
        EXPECT_EQ(wrapped(1), 2);
    auto &&actually_called_number = counter->count_of(1);
    EXPECT_EQ(actually_called_number, 1);
}

TEST_F(LRUCacheTest, CallsOnceOnMultipleArguments) {
    auto &&wrapped = LRUCache::wrap(LRUCacheTest::test_function, 2);
    int cached_calls_number = 5;
    for(auto i = 0; i < cached_calls_number; ++i) {
        EXPECT_EQ(wrapped(1), 2);
        EXPECT_EQ(wrapped(2), 4);
    }
    auto &&actually_called_number_first = counter->count_of(1);
    auto &&actually_called_number_second = counter->count_of(2);
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
    auto &&actually_called_number_first = counter->count_of(1);
    auto &&actually_called_number_second = counter->count_of(2);
    EXPECT_EQ(actually_called_number_first, cached_calls_number);
    EXPECT_EQ(actually_called_number_second, cached_calls_number);
}


TEST_F(LRUCacheTest, EvictsLeastRecentlyUsed) {
    auto &&wrapped = LRUCache::wrap(LRUCacheTest::test_function, 2);
    ASSERT_EQ(wrapped(1), 2);
    ASSERT_EQ(wrapped(2), 4);
    ASSERT_EQ(counter->count_of(1), 1);
    ASSERT_EQ(wrapped(3), 6);
    ASSERT_EQ(wrapped(2), 4);
    ASSERT_EQ(counter->count_of(2), 1);
    ASSERT_EQ(wrapped(1), 2);
    ASSERT_EQ(counter->count_of(1), 2);
}