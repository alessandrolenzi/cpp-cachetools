#include <gtest/gtest.h>
#include "cpp_cachetools/cpp_cache_tools.hpp"
#include "tests/counter.h"

TEST(LRUCacheWrapLambda, WorksOnDecayedLambda) {
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

TEST(LRUCacheWrapLambda, WorksOnAutoLambda) {
    auto my_lambda = [](int value, std::shared_ptr<CallCounter> counter){
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


TEST(LRUCacheWrapLambda, WorksOnMutableLambda) { //At your own risk...
    int called = 0;
    auto my_lambda = [&called](int value, std::shared_ptr<CallCounter> counter)mutable {
        counter->increment(value);
        called +=1;
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

TEST(LRUCacheWrapLambda, WorksWithVoidReturningLambda) {
    auto &&counter = std::make_shared<CallCounter>();
    auto&& wrapped = LRUCache::wrap([](int value, std::shared_ptr<CallCounter> counter){
        counter->increment(value);
    }, 2);
    EXPECT_EQ(counter->count_of(1), 0);
    wrapped(1, counter);
    EXPECT_EQ(counter->count_of(1), 1);
    wrapped(1, counter);
    EXPECT_EQ(counter->count_of(1), 1);
}

TEST(LRUCacheWrapLambda, WorksWithNoParameterLambda) {
    auto &&counter = std::make_shared<CallCounter>();
    auto lambda = [counter](){
        counter->increment(1);
        return counter->count_of(1);
    };
    auto&& wrapped = LRUCache::wrap(lambda, 2);
    EXPECT_EQ(counter->count_of(1), 0);
    EXPECT_EQ(wrapped(), 1);
    EXPECT_EQ(counter->count_of(1), 1);
    EXPECT_EQ(wrapped(), 1);
    EXPECT_EQ(counter->count_of(1), 1);
}


TEST(LRUCAcheWrapLambda, WorksWithVoidVoidLambda) {
    auto &&counter = std::make_shared<CallCounter>();
    auto lambda = [counter](){
        counter->increment(1);
    };
    auto&& wrapped = LRUCache::wrap(lambda, 2);
    EXPECT_EQ(counter->count_of(1), 0);
    wrapped();
    EXPECT_EQ(counter->count_of(1), 1);
    wrapped();
    EXPECT_EQ(counter->count_of(1), 1);
}