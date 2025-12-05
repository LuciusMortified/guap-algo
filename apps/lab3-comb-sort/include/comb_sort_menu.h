#pragma once

#ifndef GUAP_ALGO_COMB_SORT_MENU_H
#define GUAP_ALGO_COMB_SORT_MENU_H

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <optional>
#include <print>
#include <string>
#include <vector>

#include "comb_sort.h"

struct comb_sort_menu {
    std::vector<int> seq_;
    bool is_running_ = true;

    struct menu_action {
        std::string key;
        std::string help;
        std::function<void()> func;
    };

    std::vector<menu_action> actions_ = {
        {"a", "Добавить элемент", [this] { add_element_(); }},
        {"d", "Удалить элемент", [this] { remove_element_(); }},
        {"k", "Найти k-ое по порядку число", [this] { find_k_element_(); }},
        {"p", "Вывести все элементы", [this] { print_seq_(); }},
        {"s", "Вывести отсортированные элементы", [this] { print_sort_seq_(); }},
        {"q", "Выход", [this] { is_running_ = false; }},
        {"h", "Показать меню", [this] { print_menu_(); }},
    };

    void print_menu_() {
        for (const auto& action : actions_) {
            std::println("{}. {}", action.key, action.help);
        }
        std::println();
    }

    template <typename T>
    std::optional<T> strict_scan() {
        T value;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода.\n";
            return {};
        }
        return {value};
    }

    std::string request_choice_() {
        std::optional<std::string> choice;
        while (!choice.has_value()) {
            std::print(": ");
            choice = strict_scan<std::string>();
            if (!choice.has_value()) {
                print_menu_();
            }
        }
        return choice.value();
    }

    void add_element_() {
        std::println("Введите значение (целое число)");
        std::print("> ");
        std::optional<int> number = strict_scan<int>();
        if (!number.has_value()) {
            return;
        }
        seq_.push_back(number.value());
    }

    void remove_element_() {
        if (seq_.empty()) {
            std::println("Нет элементов.");
            return;
        }

        std::println("Введите индекс элемента для удаления");
        std::print("> ");
        std::optional<size_t> index = strict_scan<size_t>();
        if (!index.has_value()) {
            return;
        }
        if (index.value() >= seq_.size()) {
            std::println("Индекс вне диапазона.");
            return;
        }
        seq_.erase(seq_.begin() + index.value());
    }

    void find_k_element_() {
        if (seq_.empty()) {
            std::println("Нет элементов.");
            return;
        }

        std::println("Введите индекс k");
        std::print("> ");
        std::optional<size_t> k = strict_scan<size_t>();
        if (!k.has_value()) {
            return;
        }
        if (k.value() >= seq_.size()) {
            std::println("Индекс вне диапазона.");
            return;
        }

        auto seq_copy = seq_;
        comb_sorter sorter;
        sorter(seq_copy);

        std::println("Элемент с индексом {} после сортировки {}", k.value(), seq_copy[k.value()]);
        std::println();
        std::println("Статистика сортировки");
        std::println("Количество сравнений: {}", sorter.comp_count);
        std::println("Количество перестановок: {}", sorter.swap_count);
    }

    void print_seq_() {
        if (seq_.empty()) {
            std::println("Нет элементов.");
            return;
        }

        for (auto v : seq_) {
            std::print("{} ", v);
        }
        std::println();
    }

    void print_sort_seq_() {
        if (seq_.empty()) {
            std::println("Нет элементов.");
            return;
        }

        auto seq_copy = seq_;
        comb_sorter sorter;
        sorter(seq_copy);

        for (auto v : seq_copy) {
            std::print("{} ", v);
        }
        std::println();
        std::println("Статистика сортировки");
        std::println("Количество сравнений: {}", sorter.comp_count);
        std::println("Количество перестановок: {}", sorter.swap_count);
    }

    int run() {
        for (int i = 0; i < 100; i++) {
            seq_.push_back(std::rand() % 100);
        }

        print_menu_();
        while (is_running_) {
            auto choice    = request_choice_();
            auto action_it = std::ranges::find(actions_, choice, &menu_action::key);
            if (action_it != actions_.end()) {
                action_it->func();
            } else {
                std::println("Некорректный выбор. Попробуйте снова.");
                print_menu_();
            }
        }
        return 0;
    }
};

#endif  // GUAP_ALGO_COMB_SORT_MENU_H
