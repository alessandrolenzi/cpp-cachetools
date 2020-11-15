
#include "../cpp_cachetools/cpp_cache_tools.hpp"

auto fibonacci_cache = LRUCache::build<std::size_t, std::size_t>(2) ;

std::size_t fib(std::size_t n) {
    if(auto retrieved = fibonacci_cache->get(n)) {
        return *retrieved;
    }
    if (n == 0) return 0;
    if (n == 1) return 1;
    auto retval = fib(n-2) + fib(n-1);
    fibonacci_cache->insert(n, retval);
    return retval;
}

int main() {
    std::cout << fib(36) << std::endl;
    return 0;
}
