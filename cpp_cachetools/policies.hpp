#ifndef CPP_CACHETOOLS_POLICIES_HPP
#define CPP_CACHETOOLS_POLICIES_HPP
#include <optional>
#include <list>
#include <chrono>
namespace policies {

    template<typename KeyType, typename ValueType>
    struct PolicyInterface {
        PolicyInterface(unsigned int max_size): max_size{max_size} {};
        virtual const std::optional<ValueType> get(const KeyType &arguments) = 0;
        virtual void insert(const KeyType& arguments, const ValueType& value) = 0;

    protected:
        const unsigned int max_size;

    };


    template<typename K, typename V, template<typename, typename> typename I>
    struct LRU: PolicyInterface<K, V> {
        LRU(unsigned int max_size = 0) : PolicyInterface<K, V>{max_size} {}
        virtual const std::optional<V> get(const K &arguments) override {
            auto list_reference = cache.find(arguments);
            if (!list_reference)
                return {};
            auto list_element = **list_reference;
            auto retval = list_element.second;
            cache.erase(arguments);
            storage.erase(*list_reference);
            auto new_position = storage.emplace(storage.begin(), std::move(list_element));
            cache.emplace(std::move(arguments), new_position);
            return retval;
        }

        virtual void insert(const K& key, const V& value) override {
            auto found = cache.find(key);
            if (found) {
                return;
            }
            if (this->max_size != 0 && storage.size() == this->max_size) {
                evict();
            }
            auto reference = storage.emplace(storage.begin(), std::make_pair(key, value));
            cache.emplace(key, reference);
        }

    private:

        void evict() {
            auto last = storage.back();
            auto key = last.first;
            storage.pop_back();
            cache.erase(key);
        }
        std::list<std::pair<K, V>> storage;
        I<K, typename std::list<std::pair<K, V>>::iterator> cache;
    };

    template<typename clock>
    struct TTL {
        template<typename K, typename V, template<typename, typename> typename I>
        struct Class: PolicyInterface<K, V> {
            using element_type = std::tuple<K, V, std::chrono::time_point<clock>>;

            Class(unsigned int max_size, const typename clock::duration& duration) : PolicyInterface<K, V>{max_size}, duration{duration}
            {}

            virtual const std::optional<V> get(const K &arguments) override{
                auto list_reference = cache.find(arguments);
                if (!list_reference) {
                    return {};
                }
                auto list_element = **list_reference;
                if (this->expired(list_element)) {
                    this->clean_expired();
                    return {};
                }
                auto retval = std::get<1>(list_element);
                return retval;
            }


            virtual void insert(const K &key, const V &value) override{
                using namespace std::chrono_literals;
                auto found = cache.find(key);
                if (found) {
                    return;
                }
                if (this->max_size != 0 && storage.size() == this->max_size) {
                    evict();
                }
                auto expiration = clock::now() + duration;
                auto reference = storage.insert(storage.end(), std::make_tuple(key, value, expiration));
                cache.emplace(key, reference);
            }

        private:

            bool expired(const element_type &element) const {
                return clock::now() > std::get<2>(element);
            }

            void clean_expired() {
                auto iterator = storage.begin();
                while (iterator != storage.end() && this->expired(*iterator)) {
                    auto key = std::get<0>(*iterator);
                    cache.erase(key);
                    iterator = storage.erase(iterator);
                }
            }

            void evict() {
                auto current_size = storage.size();
                this->clean_expired();
                if (current_size == storage.size()) { // no expired element; get rid of the first one.
                    auto key = std::get<0>(*(storage.begin()));
                    cache.erase(key);
                    storage.erase(storage.begin());
                }
            }

            std::list<element_type> storage;
            I<K, typename std::list<element_type>::iterator> cache;
            typename clock::duration duration;
        };
    };


    template<template<typename, typename, template<typename, typename> typename> typename Policy>
    struct Builder{

        template<template<typename, typename> typename I>
        struct with_index {
            template<typename K, typename V>
            struct Class : public Policy<K, V, I> {

                template<typename... Args>
                Class(unsigned int m, Args&&... args) : Policy<K, V, I>{m, std::forward<Args&&...>(args)...} {}
            };
        };
    };

}

#endif //CPP_CACHETOOLS_POLICIES_HPP
