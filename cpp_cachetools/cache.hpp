#ifndef CPP_CACHETOOLS_CACHE_HPP
#define CPP_CACHETOOLS_CACHE_HPP
#include <functional>
#include <mutex>
#include "details.hpp"


template<template<typename, typename> class CacheContainer>
struct Cache {
    template<typename R, typename... Args, typename... CacheBuildingArgs>
    static const std::function<R(Args...)> wrap(std::function<R(Args...)> function, CacheBuildingArgs&&... cache_args) {
        auto cache = std::make_shared<CacheContainer<std::tuple<Args...>, R>>(std::forward<CacheBuildingArgs&&>(cache_args)...);
        auto mutex = std::make_shared<std::mutex>();
        return details::_inner_decorator(function, cache, mutex);
    }

    template<typename... Args,  typename... CacheBuildingArgs>
    static const std::function<void(Args...)> wrap(std::function<void(Args...)> function, CacheBuildingArgs&&... cache_args) {
        auto cache = std::make_shared<CacheContainer<std::tuple<Args...>, bool>>(std::forward<CacheBuildingArgs&&>(cache_args)...);
        auto mutex = std::make_shared<std::mutex>();
        return details::_inner_void_decorator(function, cache, mutex);
    }

    template<typename R, typename... CacheBuildingArgs>
    static const std::function<R(void)> wrap(std::function<R(void)> function, CacheBuildingArgs&&... cache_args) {
        auto cache = std::make_shared<CacheContainer<bool, R>>(std::forward<CacheBuildingArgs&&>(cache_args)...);
        auto mutex = std::make_shared<std::mutex>();
        return details::_inner_void_param_decorator(function, cache, mutex);
    }

    template<typename... CacheBuildingArgs>
    static const std::function<void(void)> wrap(std::function<void(void)> function, CacheBuildingArgs&&... cache_args) {
        auto cache = std::make_shared<CacheContainer<bool, bool>>(std::forward<CacheBuildingArgs&&>(cache_args)...);
        auto mutex = std::make_shared<std::mutex>();
        return details::_inner_all_void_decorator(function, cache, mutex);
    }

    template<typename R, typename... Args, typename... CacheBuildingArgs>
    static const std::function<R(Args...)> wrap(R (*function)(Args...), CacheBuildingArgs&&... cache_args) {
        std::function<R(Args...)> _function = function;
        return wrap(_function, std::forward<CacheBuildingArgs&&>(cache_args)...);
    }

    template<typename... Args, typename... CacheBuildingArgs>
    static const std::function<void(Args...)> wrap(void (*function)(Args...), CacheBuildingArgs&&... cache_args) {
        std::function<void(Args...)> _function = function;
        return wrap(_function, std::forward<CacheBuildingArgs&&>(cache_args)...);
    }

    template<typename R, typename... Args, typename... CacheBuildingArgs>
    static const auto build(CacheBuildingArgs&&... cache_args) {
        return std::make_shared<CacheContainer<std::tuple<Args...>, R>>(std::forward<CacheBuildingArgs&&>(cache_args)...);
    }
};

#endif //CPP_CACHETOOLS_CACHE_HPP
