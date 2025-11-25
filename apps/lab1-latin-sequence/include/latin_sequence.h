#pragma once

#ifndef GUAP_ALGO_LATIN_SEQUENCE_H
#define GUAP_ALGO_LATIN_SEQUENCE_H

#include <iostream>

#include "two_linked_list.h"

inline bool is_latin_letter(char letter) {
    return ('A' <= letter && letter <= 'Z') || ('a' <= letter && letter <= 'z');
}

struct latin_sequence {
private:
    two_linked_list<char> char_list_;
    char remover_ = '@';
    char ender_   = '.';

public:
    latin_sequence() = default;

    void add(char letter) {
        if (!is_latin_letter(letter) && letter != remover_ && letter != ender_) {
            std::cout << "Недопустимый символ: " << letter << "\n";
            return;
        }

        if (is_completed()) {
            std::cout << "Последовательность уже завершена.\n";
            return;
        }

        char_list_.push_back(letter);
    }

    bool is_empty() {
        return char_list_.is_empty();
    }

    void remove_last() {
        char_list_.pop_back();
    }

    void remove_at(int index) {
        char_list_.pop_at(index);
    }

    bool is_completed() {
        return !char_list_.is_empty() && char_list_.back() == ender_;
    }

    two_linked_list<char> list() {
        return char_list_;
    }

    two_linked_list<char> compile() {
        two_linked_list<char> result;

        for (char v : char_list_) {
            if (v == remover_) {
                result.pop_back();
            } else {
                result.push_back(v);
            }
        }

        return result;
    }
};

#endif  // GUAP_ALGO_LATIN_SEQUENCE_H
