# CPP-CacheTools
**Warning:** still in active development! Use for production at your own risk.
CPP-CacheTools is a header only library (C++17, older versions are untested) to simplify caching in CPP. It is built using the usual concept of *zero cost abstraction*. It's hightly customisable thanks to its modular design.

## Using the cache

So far, there are two ways to use the caches provided by this library:
1. Wrap a function (similarly to Python's lru_cache)
2. Use the cache explicitly.

For the first case (see `examples/function_wrap.cpp`), it's as simple as:
```
std::size_t foo(std::size_t bar) {
    std::cout << "Received bar=" << bar << std::endl;
    return bar * 2;
}
//Create LRU cache of size 2
auto cached_foo = LRUCache::wrap(foo, 2);

int main() {
    cached_foo(1);
    cached_foo(1);
    return 0;
}
``` 
In this case, the library takes care of thread safety for you. 

In the second case, you can access the cache through two simple methods, `get` and `insert`.


##Contributing

Please reach out if you're looking to help! 