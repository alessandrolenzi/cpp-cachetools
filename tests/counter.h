#ifndef CPP_CACHETOOLS_TEST_COUNTER_H
#define CPP_CACHETOOLS_TEST_COUNTER_H
#include <unordered_map>
#include <functional>

struct CallCounter {
    void increment(int value) {
        auto&& called_iter = counter.find(value);
        if(called_iter == counter.end()) {
            counter.emplace(value, 1);
        } else {
            auto &&number_of_calls = called_iter->second;
            counter.insert_or_assign(called_iter, value, number_of_calls + 1);
        }
    }

    int count_of(int value) {
        auto&& called_iter = counter.find(value);
        if(called_iter == counter.end()) {
            return 0;
        }
        return called_iter->second;
    }

    std::unordered_map<int, int> counter{};
};

namespace std {
    template <> struct hash<std::shared_ptr<CallCounter>>
    {
        size_t operator()(const std::shared_ptr<CallCounter> &x) const
        {
            return 0;
        }
    };
}

#endif //CPP_CACHETOOLS_TEST_COUNTER_H
