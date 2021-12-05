#include <gtest/gtest.h>
#include "cpp_cachetools/cpp_cache_tools.hpp"
#include "tests/counter.h"

TEST(LRUCacheWrapLambda, Works) {
    std::function<int(int, std::shared_ptr<CallCounter>)> my_lambda = [](int value, std::shared_ptr<CallCounter> counter){
        counter->increment(value);
        return value * 2;
    };
    auto counter = std::make_shared<CallCounter>();
    auto &&wrapped = LRUCache::wrap(my_lambda, 2);
    EXPECT_EQ(counter->count_of(2), 0);
    auto retrieved = wrapped(2, counter);
    EXPECT_EQ(retrieved, 4);
    wrapped(2, counter);
    EXPECT_EQ(counter->count_of(2), 1);
}