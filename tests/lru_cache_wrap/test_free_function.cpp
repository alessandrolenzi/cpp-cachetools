#include <memory>
#include <gtest/gtest.h>
#include "tests/counter.h"
#include "cpp_cachetools/cpp_cache_tools.hpp"


int my_function(int value, std::shared_ptr<CallCounter> counter) {
    counter->increment(value);
    return value *2;
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