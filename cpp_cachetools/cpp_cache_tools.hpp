#ifndef CPP_CACHE_TOOLS_HPP
#define CPP_CACHE_TOOLS_HPP

#include "indexes.hpp"
#include "details.hpp"
#include "policies.hpp"
#include "cache.hpp"


struct LRUCache: Cache<policies::Builder<policies::LRU>::with_index<indexes::HashedIndex>::Class> {};
#define LRUCACHE_MEMBER_FUNCTION(SIZE, TYPE, NAME, PARAMS, BODY) std::function<TYPE PARAMS> NAME = LRUCache::wrap([this] PARAMS BODY, SIZE);

struct TTLCache: Cache<policies::Builder<policies::TTL<std::chrono::steady_clock>::Class>::with_index<indexes::HashedIndex>::Class> {};

#endif //CPP_CACHE_TOOLS_HPP
