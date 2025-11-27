#pragma once

#ifndef GUAP_ALGO_LIST_H
#define GUAP_ALGO_LIST_H

#include <utility>
#include <vector>

template <typename T>
struct hash_bucket final {
private:
    struct node final {
        T value;
        node* next = nullptr;
        node* prev = nullptr;
    };

    node* head_ = nullptr;
    node* tail_ = nullptr;
    int size_   = 0;

    void remove_node_(node* target) {
        if (target->prev) {
            auto* prev = target->prev;
            prev->next = target->next;
        }
        if (target->next) {
            auto* next = target->next;
            next->prev = target->prev;
        }
        if (target == head_) {
            head_ = target->next;
        }
        if (target == tail_) {
            tail_ = target->prev;
        }

        delete target;
    }

public:
    hash_bucket() = default;
    ~hash_bucket() {
        while (head_) {
            pop_back();
        }
    }

    hash_bucket& operator=(const hash_bucket& rhs) {
        if (this == &rhs) {
            return *this;
        }

        clear();
        for (T value : rhs) {
            push_back(value);
        }

        return *this;
    }
    hash_bucket(const hash_bucket& rhs) {
        *this = rhs;
    }

    hash_bucket& operator=(hash_bucket&& rhs) noexcept {
        if (this == &rhs) {
            return *this;
        }

        std::swap(head_, rhs.head_);
        std::swap(tail_, rhs.tail_);
        std::swap(size_, rhs.size_);

        return *this;
    }
    hash_bucket(hash_bucket&& rhs) noexcept {
        *this = std::move(rhs);
    }

    size_t size() const {
        return size_;
    }

    bool is_empty() const {
        return size_ == 0;
    }

    T& back() {
        return tail_->value;
    }

    const T& back() const {
        return tail_->value;
    }

    void push_back(const T& value) {
        auto* new_node = new node{value, nullptr, tail_};
        if (tail_) {
            tail_->next = new_node;
        } else {
            head_ = new_node;
        }
        tail_ = new_node;
        size_++;
    }

    void pop_back() {
        if (!tail_) {
            return;
        }

        auto* to_delete = tail_;
        tail_           = tail_->prev;
        if (tail_) {
            tail_->next = nullptr;
        } else {
            head_ = nullptr;
        }
        delete to_delete;
        size_--;
    }

    void pop_at(int index) {
        if (index < 0 || index >= size_) {
            return;
        }

        auto* current     = head_;
        int current_index = 0;
        while (current_index != index) {
            current = current->next;
            current_index++;
        }

        remove_node_(current);
        size_--;
    }

    void clear() {
        while (head_) {
            pop_back();
        }
    }

    struct iterator final {
        node* current;

        iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        T& operator*() {
            return current->value;
        }

        T* operator->() {
            return &current->value;
        }

        bool operator==(const iterator& rhs) {
            return current == rhs.current;
        }

        bool operator!=(const iterator& rhs) {
            return !(*this == rhs);
        }
    };

    iterator begin() {
        return {head_};
    }

    iterator end() {
        return {nullptr};
    }

    iterator erase(iterator it) {
        if (it.current == nullptr) {
            return end();
        }

        auto* next_node = it.current->next;
        remove_node_(it.current);
        size_--;

        return {next_node};
    }

    struct const_iterator final {
        node* current;

        const_iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        const T& operator*() {
            return current->value;
        }

        const T* operator->() {
            return &current->value;
        }

        bool operator==(const const_iterator& rhs) {
            return current == rhs.current;
        }

        bool operator!=(const const_iterator& rhs) {
            return !(*this == rhs);
        }
    };

    const_iterator begin() const {
        return {head_};
    }

    const_iterator end() const {
        return {nullptr};
    }
};

#endif  // GUAP_ALGO_LIST_H
