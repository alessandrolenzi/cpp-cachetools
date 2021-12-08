#include <gtest/gtest.h>

#include <utility>
#include "cpp_cachetools/cpp_cache_tools.hpp"
#include "tests/counter.h"

struct TheObject {
    TheObject(std::shared_ptr<CallCounter> counter):
        get_value_cached{LRUCache::wrap([this](int value){return this->get_value(value);}, 2)},
        call_counter{std::move(counter)}
        {}

    int get_value(int value) {
        call_counter->increment(value);
        return value *2;
    }
    std::function<int(int)> get_value_cached;
    std::shared_ptr<CallCounter> call_counter;
};

struct TheFancyObject {
    TheFancyObject(std::shared_ptr<CallCounter> call_counter) : call_counter{std::move(call_counter)} {};

    LRUCACHE_MEMBER_FUNCTION(2, float, cached_function, (int value), {
        call_counter->increment(value);
        return float(call_counter->count_of(value));
    })

private:
    std::shared_ptr<CallCounter> call_counter;
};

TEST(LRUCacheWrapMemberFunction, KeepsThisState) {
    auto counter = std::make_shared<CallCounter>();
    TheObject a{counter};
    EXPECT_EQ(a.get_value_cached(2), 4);
    EXPECT_EQ(a.get_value_cached(2), 4);
    EXPECT_EQ(a.get_value_cached(2), 4);
    ASSERT_EQ(counter->count_of(2), 1);
}

TEST(LRUCacheWrapMemberFunction, KeepsThisStateOnDecorated) {
    auto counter = std::make_shared<CallCounter>();
    TheFancyObject a{counter};
    EXPECT_EQ(a.cached_function(2), 1);
    EXPECT_EQ(a.cached_function(2), 1);
    EXPECT_EQ(a.cached_function(2), 1);
    ASSERT_EQ(counter->count_of(2), 1);
}

TEST(LRUCacheWrapMemberFunction, DoesNotConfuseDifferentInstances) {
    auto counter = std::make_shared<CallCounter>();
    TheFancyObject a{counter};
    TheFancyObject b{counter};
    EXPECT_EQ(a.cached_function(2), 1);
    EXPECT_EQ(b.cached_function(2), 2);
    EXPECT_EQ(a.cached_function(2), 1);
    ASSERT_EQ(counter->count_of(2), 2);
}