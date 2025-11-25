#pragma once

#ifndef GUAP_ALGO_LATIN_SEQUENCE_MENU_H
#define GUAP_ALGO_LATIN_SEQUENCE_MENU_H

#include <latin_sequence.h>

#include <iostream>
#include <optional>

struct latin_sequence_menu {
    latin_sequence seq;

    void print_menu() {
        std::cout
            << "a. Добавить элемент\n"
            << "d. Удалить элемент\n"
            << "p. Показать последовательность\n"
            << "c. Сформировать итоговую последовательность\n"
            << "q. Выход\n"
            << "h. Показать меню\n\n"
            << "Доп. справка:\n"
            << "Спец. символ удаления предыдущего символа - '@'\n"
            << "Спец. символ завершения последовательности - '.'\n\n";
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

    char request_choice() {
        std::optional<char> choice;
        while (!choice.has_value()) {
            std::cout << ": ";
            choice = strict_scan<char>();
            if (!choice.has_value()) {
                print_menu();
            }
        }
        return choice.value();
    }

    void add_letter() {
        std::optional<char> letter;
        while (!letter.has_value()) {
            std::cout << "Введите латинскую букву (или '@', '.')\n> ";
            letter = strict_scan<char>();
        }
        seq.add(letter.value());
        print_seq();
    }

    void remove_letter() {
        std::optional<int> index;
        while (!index.has_value()) {
            std::cout << "Введите индекс для удаления (или -1 для удаления последнего символа)\n> ";
            index = strict_scan<int>();
        }
        if (index == -1) {
            seq.remove_last();
        } else {
            seq.remove_at(index.value());
        }
        print_seq();
    }

    void print_seq() {
        if (seq.is_empty()) {
            std::cout << "Последовательность пуста.\n";
            return;
        }

        std::cout << "Последовательность: ";
        for (char c : seq.list()) {
            std::cout << c;
        }
        std::cout << "\n";
    }

    void compile_seq() {
        if (!seq.is_completed()) {
            std::cout << "Последовательность не завершена. Добавьте '.' в конец.\n";
            return;
        }
        std::cout << "Преобразованная последовательность: ";
        for (char letter : seq.compile()) {
            std::cout << letter;
        }
        std::cout << "\n";
    }

    int run() {
        print_menu();

        while (true) {
            switch (request_choice()) {
                case 'q':
                    return 0;

                case 'a':
                    add_letter();
                    break;

                case 'd':
                    remove_letter();
                    break;

                case 'p':
                    print_seq();
                    break;

                case 'c':
                    compile_seq();
                    break;

                case 'h':
                    print_menu();
                    break;

                default:
                    std::cout << "Некорректный выбор. Попробуйте снова.\n";
                    print_menu();
                    break;
            }
        }
    }
};

#endif  // GUAP_ALGO_LATIN_SEQUENCE_MENU_H
