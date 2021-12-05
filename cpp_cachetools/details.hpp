#ifndef CPP_CACHETOOLS_DETAILS_HPP
#define CPP_CACHETOOLS_DETAILS_HPP

#include <functional>
#include <mutex>
#include <memory>

namespace details {
        template<typename CacheType, typename... Args>
        constexpr std::function<void(Args...)> _inner_void_decorator(std::function<void(Args...)> function, const std::shared_ptr<CacheType>& cache, const std::shared_ptr<std::mutex>& mutex) {
            return [function = std::move(function), cache, mutex](Args&&... args) mutable {
                auto tuple = std::make_tuple(args...);
                {
                    std::lock_guard<std::mutex> lock(*mutex);
                    auto found = cache->get(tuple);
                    if (found) {
                        return;
                    }
                    cache->insert(std::move(tuple), true);
                }
                return function(std::forward<decltype(args)>(args)...);
            };
        }

        template<typename CacheType, typename R, typename... Args>
        constexpr std::function<R(Args...)> _inner_decorator(std::function<R(Args...)> function, const std::shared_ptr<CacheType>& cache,  const std::shared_ptr<std::mutex>& mutex) {
            return [function = std::move(function), cache, mutex](Args&&... args) mutable {
                auto tuple = std::make_tuple(args...);
                {
                    std::lock_guard lg{*mutex};
                    auto found = cache->get(tuple);
                    if (found) {
                        return *found;
                    }
                }
                auto retrieved = function(std::forward<decltype(args)>(args)...);
                {
                    std::lock_guard lg{*mutex};
                    if(!cache->get(tuple))
                        cache->insert(std::move(tuple), retrieved);
                    return retrieved;
                }
            };
        }


        template<typename CacheType, typename R>
        constexpr std::function<R(void)> _inner_void_param_decorator(std::function<R(void)> function, const std::shared_ptr<CacheType>& cache,  const std::shared_ptr<std::mutex>& mutex) {
            return [function = std::move(function), cache, mutex]() mutable {
                bool key = true;
                {
                    auto found = cache->get(key);
                    if (found) {
                        return *found;
                    }
                }
                auto retrieved = function();
                {
                    cache->insert(key, retrieved);
                    return retrieved;
                }
            };
        }


        template<typename CacheType, typename R>
        constexpr std::function<R(void)> _inner_all_void_decorator(std::function<R(void)> function, const std::shared_ptr<CacheType>& cache,  const std::shared_ptr<std::mutex>& mutex) {
            return [function = std::move(function), cache, mutex]() mutable {
                bool key = true;
                {
                    std::lock_guard<std::mutex> lock(*mutex);
                    auto found = cache->get(key);
                    if (found) {
                        return;
                    }
                    function();
                    cache->insert(key, true);
                    return;
                }
            };
        }

        template<typename A>
        constexpr auto hash_helper(const A& a) {
            return std::hash<A>()(a);
        }

        template<typename T>
        struct hash: std::hash<T> {};
        template<int Size, int Index, typename... Args>
        struct _hash {
            std::size_t operator()(const std::tuple<Args...>& tuple) {
                return hash_helper(std::get<Index>(tuple)) + _hash<Size, Index+1, Args...>()(tuple);
            }
        };

        template<int Size, typename...Args>
        struct _hash<Size, Size-1, Args...> {
            std::size_t operator()(const std::tuple<Args...> &tuple) {
                return hash_helper(std::get<Size-1>(tuple));
            }
        };

        template<typename T>
        struct hash<std::tuple<T>>{
            constexpr std::size_t operator()(const std::tuple<T>& k) const {
                return std::hash<T>()(std::get<0>(k));
            }
        };

        template<>
        struct hash<std::tuple<>>{
            constexpr std::size_t operator()(const std::tuple<void>& k) const {
                return 0;
            }
        };

        template<typename... Args>
        struct hash<std::tuple<Args...>> {
            constexpr std::size_t operator()(const std::tuple<Args...> &k) const{
                return _hash<std::tuple_size<std::tuple<Args...>>::value, 0, Args...>()(k);
            }
        };

    };


#endif //CPP_CACHETOOLS_DETAILS_HPP
