#include "../cpp_cachetools/cpp_cache_tools.hpp"

std::size_t foo(std::size_t bar) {
    std::cout << "Received bar=" << bar << std::endl;
    return bar * 2;
}
auto cached_foo = LRUCache::wrap(foo, 2);

int main() {
    cached_foo(1);
    cached_foo(1);
    return 0;
}