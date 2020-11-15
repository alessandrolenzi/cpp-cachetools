#ifndef CPP_CACHETOOLS_INDEXES_HPP
#define CPP_CACHETOOLS_INDEXES_HPP
#include <functional>
#include <iostream>
#include <unordered_map>
#include <list>
#include <mutex>
#include <chrono>
#include "details.hpp"

namespace indexes {
    template<typename K, typename StoredReference>
    struct HashedIndex {
        std::optional<StoredReference> find(const K &k) {
            auto it = index.find(k);
            if (it != index.end())
                return it->second;
            return {};
        }

        void emplace(const K &k, const StoredReference &r) {
            index.emplace(k, r);
        }

        void erase(const K &k) {
            auto it = index.find(k);
            if (it != index.end())
                index.erase(it);
        }

    private:
        std::unordered_map<K, StoredReference, details::hash<K>> index;
    };
}

#endif //CPP_CACHETOOLS_INDEXES_HPP
