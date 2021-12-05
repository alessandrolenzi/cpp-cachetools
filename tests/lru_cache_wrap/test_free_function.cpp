#include <memory>
#include <gtest/gtest.h>
#include "tests/counter.h"
#include "cpp_cachetools/cpp_cache_tools.hpp"


int my_function(int value, std::shared_ptr<CallCounter> counter) {
    counter->increment(value);
    return value *2;
}

void a_void_function(int value, std::shared_ptr<CallCounter> counter) {
    counter->increment(value);
}

std::shared_ptr<CallCounter> default_counter{};

int a_void_int_function() {
    default_counter->increment(1);
    return default_counter->count_of(1);
}

TEST(LRUCacheFreeFunction, CallsOnlyOnce) {
    auto &&counter = std::make_shared<CallCounter>();
    auto &&wrapped = LRUCache::wrap(my_function, 2);
    EXPECT_EQ(counter->count_of(2), 0);
    EXPECT_EQ(wrapped(2, counter), 4);
    EXPECT_EQ(counter->count_of(2), 1);
    EXPECT_EQ(wrapped(2, counter), 4);
    EXPECT_EQ(counter->count_of(2), 1);
}

TEST(LRUCacheFreeFunction, CallsOnlyOnceWithVoidReturn) {
    auto &&counter = std::make_shared<CallCounter>();
    auto &&wrapped = LRUCache::wrap(a_void_function, 2);
    EXPECT_EQ(counter->count_of(2), 0);
    wrapped(2, counter);
    EXPECT_EQ(counter->count_of(2), 1);
    wrapped(2, counter);
    EXPECT_EQ(counter->count_of(2), 1);
}

TEST(LRUCacheFreeFunction, CallsOnlyOnceWithNoParameter) {
    default_counter = std::make_shared<CallCounter>();
    auto &&wrapped = LRUCache::wrap(a_void_int_function, 2);
    EXPECT_EQ(default_counter->count_of(1), 0);
    EXPECT_EQ(wrapped(), 1);
    EXPECT_EQ(default_counter->count_of(1), 1);
    EXPECT_EQ(wrapped(), 1);
    EXPECT_EQ(default_counter->count_of(1), 1);
}