#pragma once

#ifndef GUAP_ALGO_HASH_MAP_H
#define GUAP_ALGO_HASH_MAP_H

#include <stdexcept>

#include "hash_bucket.h"
#include "hash_key.h"

template <typename V>
struct hash_table {
    static constexpr size_t bucket_count = 1500;

    using key_type = std::array<char, key_size>;

    struct item {
        key_type key;
        V value;
    };

private:
    hash_bucket<item> buckets_[bucket_count] = {};

    size_t bucket_by_key_(key_type key) const {
        return hash_key(key) % bucket_count;
    }

public:
    V& operator[](key_type key) {
        auto& bucket = buckets_[bucket_by_key_(key)];
        for (auto& it : bucket) {
            if (it.key == key) {
                return it.value;
            }
        }
        bucket.push_back({key, V{}});
        return bucket.back().value;
    }

    const V& operator[](key_type key) const {
        for (const auto& it : buckets_[bucket_by_key_(key)]) {
            if (it.key == key) {
                return it.value;
            }
        }
        throw std::out_of_range("key not found");
    }

    void insert(key_type key, V value) {
        auto& bucket = buckets_[bucket_by_key_(key)];
        for (auto& it : bucket) {
            if (it.key == key) {
                it.value = value;
                return;
            }
        }
        bucket.push_back({key, value});
    }

    bool contains(key_type key) const {
        for (const auto& it : buckets_[bucket_by_key_(key)]) {
            if (it.key == key) {
                return true;
            }
        }
        return false;
    }

    void remove(key_type key) {
        hash_bucket<item>& bucket = buckets_[bucket_by_key_(key)];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                return;
            }
        }
    }

    hash_bucket<item>& bucket(size_t index) {
        return buckets_[index];
    }

    const hash_bucket<item>& bucket(size_t index) const {
        return buckets_[index];
    }
};

#endif  // GUAP_ALGO_HASH_MAP_H
