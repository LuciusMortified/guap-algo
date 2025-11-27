#pragma once

#ifndef GUAP_ALGO_HASH_H
#define GUAP_ALGO_HASH_H

#include <array>
#include <optional>
#include <string>

inline constexpr size_t key_size = 6;

inline size_t bad_hash_key(std::array<char, key_size> key) {
    size_t h = 1;
    h *= key[0] - 'A';
    h *= key[1] - '0';
    h *= key[2] - '0';
    h *= key[3] - '0';
    h *= key[4] - 'A';
    h *= key[5] - 'A';

    return h;
}

inline size_t hash_key(std::array<char, key_size> key) {
    constexpr size_t p = 131ull;
    constexpr size_t m = 1000000007ull;

    size_t h = 0;

    h = (key[0] - 'A') % m;
    h = (h * p + (key[1] - '0')) % m;
    h = (h * p + (key[2] - '0')) % m;
    h = (h * p + (key[3] - '0')) % m;
    h = (h * p + (key[4] - 'A')) % m;
    h = (h * p + (key[5] - 'A')) % m;

    return h;
}

inline bool is_letter(char c) {
    return c >= 'A' && c <= 'Z';
}

inline bool is_number(char c) {
    return c >= '0' && c <= '9';
}

inline bool is_valid_key(std::array<char, key_size> key) {
    return (
        is_letter(key[0]) &&  //
        is_number(key[1]) &&  //
        is_number(key[2]) &&  //
        is_number(key[3]) &&  //
        is_letter(key[4]) &&  //
        is_letter(key[5])
    );
}

struct key_gen {
private:
    static inline const std::string format_ = "A000AA";
    std::array<char, key_size> value_       = {'A', '0', '0', '0', 'A', 'A'};

    void advance_() {
        value_[key_size - 1]++;
        for (int i = key_size - 1; i >= 0; i--) {
            if (format_[i] == 'A' && value_[i] > 'Z' && i > 0) {
                value_[i] -= 'Z' - 'A';
                value_[i - 1]++;
            }
            if (format_[i] == '0' && value_[i] > '9' && i > 0) {
                value_[i] -= '9' - '0';
                value_[i - 1]++;
            }
        }
    }

public:
    std::optional<std::array<char, key_size>> next() {
        if (auto key = value_; is_valid_key(key)) {
            advance_();
            return {key};
        }
        return {};
    }
};

#endif  // GUAP_ALGO_HASH_H
