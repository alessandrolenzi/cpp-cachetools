#include "../cpp_cachetools/cpp_cache_tools.hpp"

#include <iostream>
#include <algorithm>

std::size_t fib(std::size_t n, std::function<std::size_t(std::size_t)> _fib){
    if(n == 0) {
        return 0;
    }
    if(n == 1) return 1;
    auto f1 = _fib(n-2);
    auto f2 = _fib(n-1);
    return f1 + f2;
};


int main() {
    std::function<std::size_t(std::size_t)> bound_fib = [&bound_fib](std::size_t n){
        return fib(n, bound_fib);
    };
    bound_fib = LRUCache::wrap(bound_fib, 2);
    std::cout << bound_fib(36) << std::endl;
    return 0;
}
