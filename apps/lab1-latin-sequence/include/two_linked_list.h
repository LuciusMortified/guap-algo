#pragma once

#ifndef GUAP_ALGO_LIST_H
#define GUAP_ALGO_LIST_H

template <typename T>
struct node final {
    T value;
    node* next = nullptr;
    node* prev = nullptr;
};

template <typename T>
struct two_linked_list final {
private:
    node<T>* head_ = nullptr;
    node<T>* tail_ = nullptr;
    int size_      = 0;

    void remove_node_(node<T>* target) {
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
    two_linked_list() = default;
    ~two_linked_list() {
        while (head_) {
            pop_back();
        }
    }

    two_linked_list& operator=(const two_linked_list& rhs) {
        if (this == &rhs) {
            return *this;
        }

        clear();
        for (T value : rhs) {
            push_back(value);
        }

        return *this;
    }
    two_linked_list(const two_linked_list& rhs) {
        *this = rhs;
    }

    two_linked_list& operator=(two_linked_list&& rhs) noexcept {
        if (this == &rhs) {
            return *this;
        }

        std::swap(head_, rhs.head_);
        std::swap(tail_, rhs.tail_);
        std::swap(size_, rhs.size_);

        return *this;
    }
    two_linked_list(two_linked_list&& rhs) noexcept {
        *this = std::move(rhs);
    }

    bool is_empty() const {
        return size_ == 0;
    }

    T back() const {
        return tail_->value;
    }

    void push_back(const T& value) {
        auto* new_node = new node<T>{value, nullptr, tail_};
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
        node<T>* current;

        iterator& operator++() {
            if (current) {
                current = current->next;
            }
            return *this;
        }

        T& operator*() {
            return current->value;
        }

        bool operator==(const iterator& rhs) {
            return current == rhs.current;
        }

        bool operator!=(const iterator& rhs) {
            return !(*this == rhs);
        }
    };

    iterator begin() const {
        return {head_};
    }

    iterator end() const {
        return {nullptr};
    }
};

#endif  // GUAP_ALGO_LIST_H
