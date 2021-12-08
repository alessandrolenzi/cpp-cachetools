#include <gtest/gtest.h>
#include "cpp_cachetools/policies.hpp"
#include "cpp_cachetools/indexes.hpp"
#include "cpp_cachetools/cache.hpp"
#include "../fake_clock/fake_clock.hh"

struct TTLTestCache: Cache<policies::Builder<policies::TTL<testing::fake_clock>::Class>::with_index<indexes::HashedIndex>::Class> {};
using namespace std::chrono_literals;

TEST(TTLCache, EvictsWhenTimeIsOver) {
    auto cache_duration = testing::fake_clock::duration(60);
    auto advance = testing::fake_clock::duration(31);
    auto &&cache = TTLTestCache::build<int, int>(10, cache_duration);
    cache->insert(1, 1);
    testing::fake_clock::advance(advance);
    auto not_expired = cache->get(1);
    EXPECT_EQ(not_expired.value(), 1);
    testing::fake_clock::advance(advance);
    auto retrieved = cache->get(1);
    EXPECT_EQ(retrieved, std::optional<int>{});
}


TEST(TTLCache, EvictsOlderFirstWhenFull) {
    auto cache_duration = testing::fake_clock::duration(60);
    auto advance = testing::fake_clock::duration(31);
    auto &&cache = TTLTestCache::build<int, int>(2, cache_duration);
    cache->insert(1, 1);
    cache->insert(2, 2);
    cache->insert(3, 3);
    auto evicted_because_old = cache->get(1);
    EXPECT_EQ(evicted_because_old, std::optional<int>{});
    testing::fake_clock::advance(advance);
    EXPECT_EQ(cache->get(2), 2);
    EXPECT_EQ(cache->get(3), 3);
}

TEST(TTLCache, EvictsExpiredFirst) {
    auto cache_duration = testing::fake_clock::duration(60);
    auto advance = testing::fake_clock::duration(31);
    auto &&cache = TTLTestCache::build<int, int>(2, cache_duration);
    cache->insert(1, 1);
    testing::fake_clock::advance(advance);
    cache->insert(2, 2);
    testing::fake_clock::advance(advance);
    cache->insert(3, 3);
    auto evicted_because_old = cache->get(1);
    EXPECT_EQ(evicted_because_old, std::optional<int>{});
    EXPECT_EQ(cache->get(2), 2);
    testing::fake_clock::advance(advance);
    EXPECT_EQ(cache->get(3), 3);
}
