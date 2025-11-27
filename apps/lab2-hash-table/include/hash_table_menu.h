#pragma once

#ifndef GUAP_ALGO_MENU_H
#define GUAP_ALGO_MENU_H

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <print>

#include "hash_key.h"
#include "hash_table.h"

struct hash_table_menu {
private:
    hash_table<int> table_ = {};
    bool is_running_       = true;

    std::unordered_map<std::string, std::function<void()>> actions_ = {
        {"a", [this] { add_element_(); }},
        {"f", [this] { find_element_(); }},
        {"d", [this] { remove_element_(); }},
        {"p", [this] { print_table_(); }},
        {"pp", [this] { dump_table_(); }},
        {"i", [this] { print_hash_analysis_(); }},
        {"h", [this] { print_menu_(); }},
        {"q", [this] { is_running_ = false; }},
    };

    void print_menu_() {
        std::println("a. Добавить элемент");
        std::println("f. Найти элемент");
        std::println("d. Удалить элемент");
        std::println("p. Вывести содержимое на экран");
        std::println("i. Проанализировать качество хэш функции");
        std::println("q. Выход");
        std::println("h. Показать меню");
        std::println();
        std::println("Формат ключа для хэш таблицы - 'A000AA'");
        std::println("Количества бакетов в таблице - {}", hash_table<int>::bucket_count);
    }

    template <typename T>
    std::optional<T> strict_scan() {
        T value;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода. Попробуйте снова.\n";
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

    std::array<char, key_size> request_key_() {
        std::println("Введите ключ (формат 'A000AA')");
        std::optional<std::string> input_key;
        std::array<char, key_size> key = {};

        while (!input_key.has_value()) {
            std::print("> ");
            input_key = strict_scan<std::string>();

            if (input_key.has_value()) {
                if (auto key_str = input_key.value(); key_str.size() != key_size) {
                    input_key.reset();
                } else {
                    std::ranges::copy_n(key_str.begin(), key_size, key.begin());
                    if (!is_valid_key(key)) {
                        input_key.reset();
                    }
                }

                if (!input_key.has_value()) {
                    std::println("Некорректный формат ключа. Попробуйте снова.");
                }
            }
        }

        return key;
    }

    int request_value_() {
        std::println("Введите значение (целое число)");
        std::optional<int> input_value;

        while (!input_value.has_value()) {
            std::print("> ");
            input_value = strict_scan<int>();
        }

        return input_value.value();
    }

    void add_element_() {
        auto key    = request_key_();
        table_[key] = request_value_();
    }

    void find_element_() {
        if (auto key = request_key_(); table_.contains(key)) {
            std::println("Значение {}", table_[key]);
        } else {
            std::println("Элемент с таким ключом не найден.");
        }
    }

    void remove_element_() {
        table_.remove(request_key_());
    }

    void print_table_() {
        for (size_t i = 0; i < hash_table<int>::bucket_count; i++) {
            if (const auto& bucket = table_.bucket(i); !bucket.is_empty()) {
                std::println("{}", i);
                size_t j = 0;
                for (const auto& [key, value] : bucket) {
                    if (j++ < bucket.size() - 1) {
                        std::print("├");
                    } else {
                        std::print("└");
                    }
                    std::string key_str(key.begin(), key.end());
                    std::println(" {} = {}", key_str, value);
                }
                std::println();
            }
        }
    }

    void print_hash_analysis_() {
        std::println("Анализ качества хэш функции перебором всех возможных ключей");

        constexpr auto bucket_count = hash_table<int>::bucket_count;

        auto buckets     = std::array<int, bucket_count>{};
        size_t key_count = 0;

        key_gen gen{};
        while (auto result = gen.next()) {
            auto key = result.value();
            key_count++;
            buckets[hash_key(key) % bucket_count]++;
        }

        std::println("Всего ключей было сгенерировано\t\t{}", key_count);

        auto bucket_ideal = key_count / bucket_count;

        auto bucket_avg = std::accumulate(buckets.begin(), buckets.end(), 0) / buckets.size();
        auto bucket_min = *std::ranges::min_element(buckets);
        auto bucket_max = *std::ranges::max_element(buckets);

        std::println("Средняя наполненность бакета\t\t{}", bucket_avg);
        std::println("Минимальная наполненность бакета\t{}", bucket_min);
        std::println("Максимальная наполненность бакета\t{}", bucket_max);
        std::println("Идеальная наполненность бакета\t\t{}", bucket_ideal);

        auto quality = static_cast<float>(bucket_min) / bucket_ideal;
        std::println("Качество хэш функции [0..1)\t\t{}", quality);
    }

    void dump_table_() {
        std::ofstream file("dump.csv", std::ios::out);
        if (!file.is_open()) {
            std::println("Не удалось создать файл для дампа.");
            return;
        }

        for (size_t i = 0; i < hash_table<int>::bucket_count; i++) {
            if (const auto& bucket = table_.bucket(i); !bucket.is_empty()) {
                for (const auto& [key, value] : bucket) {
                    std::string key_str(key.begin(), key.end());
                    file << i << "," << key_str << "," << value << "\n";
                }
            }
        }

        std::println("Дамп хэш таблицы сохранён в файл dump.csv");
    }

public:
    int run() {
        print_menu_();
        while (is_running_) {
            if (auto choice = request_choice_(); actions_.contains(choice)) {
                actions_[choice]();
            } else {
                std::println("Некорректный выбор. Попробуйте снова.");
                print_menu_();
            }
        }
        return 0;
    }
};

#endif  // GUAP_ALGO_MENU_H
