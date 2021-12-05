#include <gtest/gtest.h>
#include <optional>
#include "cpp_cachetools/cpp_cache_tools.hpp"

struct LRUCacheTest: public ::testing::Test {
    LRUCacheTest(): cache{LRUCache::build<int, int>(2)} {}
    void SetUp() override {
        this->cache = LRUCache::build<int, int>(2);
    }

    std::decay<decltype(LRUCache::build<int, int>(1))>::type cache;
};

TEST_F(LRUCacheTest, DoesNotOverridePreviousValue) {
    EXPECT_EQ(cache->get(1), std::optional<int>{});
    cache->insert(1, 100);
    EXPECT_EQ(cache->get(1).value(), 100);
    cache->insert(1, 200);
    EXPECT_EQ(cache->get(1).value(), 100);
}

TEST_F(LRUCacheTest, RemovesLRUValues) {
    for(auto i = 0; i < 5; ++i) {
        EXPECT_EQ(cache->get(i), std::optional<int>{});
        cache->insert(i, i*2);
        EXPECT_EQ(cache->get(i).value(), i*2);
    }
    EXPECT_EQ(cache->get(4).value(), 8);
    EXPECT_EQ(cache->get(3).value(), 6);
    for(auto i = 0; i < 3; ++i) {
        EXPECT_EQ(cache->get(i), std::optional<int>{});
    }
}


TEST_F(LRUCacheTest, DoesNotDependOnOrderOfInsertion) {
    cache->insert(1, 2);
    cache->insert(2, 4);
    cache->get(2);
    cache->get(1);
    cache->insert(3, 6);
    EXPECT_EQ(cache->get(1).value(), 2);
    EXPECT_EQ(cache->get(3).value(), 6);
    EXPECT_EQ(cache->get(2), std::optional<int>{});
}