#pragma once

#ifndef GUAP_ALGO_COMB_SORT_H
#define GUAP_ALGO_COMB_SORT_H

#include <ranges>

struct comb_sorter {
    mutable size_t comp_count = 0;
    mutable size_t swap_count = 0;

    template <
        std::random_access_iterator I,
        std::sentinel_for<I> S,
        class Comp = std::ranges::less,
        class Proj = std::identity>
        requires std::sortable<I, Comp, Proj>
    constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const {
        comp_count = 0;
        swap_count = 0;

        auto n = std::ranges::distance(first, last);
        if (n < 2) {
            return last;
        }

        auto step    = n;
        bool swapped = false;

        auto comp_fn = [this, &comp, &proj]<typename T0, typename T1>(T0&& lhs, T1&& rhs) -> bool {
            ++comp_count;
            return std::invoke(
                comp,
                std::invoke(proj, std::forward<T1>(rhs)),
                std::invoke(proj, std::forward<T0>(lhs))
            );
        };
        auto swap_fn = [this]<typename T0, typename T1>(T0&& lhs, T1&& rhs) {
            ++swap_count;
            std::ranges::iter_swap(std::forward<T0>(lhs), std::forward<T1>(rhs));
        };

        while (step > 1 || swapped) {
            if (step > 1) {
                step = step * 10 / 13;
            }
            swapped = false;

            decltype(step) i = 0;
            while (i + step < n) {
                auto left  = std::ranges::next(first, i);
                auto right = std::ranges::next(first, i + step);
                if (comp_fn(*left, *right)) {
                    swap_fn(left, right);
                    swapped = true;
                }
                i += step;
            }
        }

        return last;
    }

    template <
        std::ranges::random_access_range R,
        class Comp = std::ranges::less,
        class Proj = std::identity>
        requires std::sortable<std::ranges::iterator_t<R>, Comp, Proj>
    constexpr std::ranges::borrowed_iterator_t<R> operator()(
        R&& r, Comp comp = {}, Proj proj = {}
    ) const {
        return (*this)(
            std::ranges::begin(r), std::ranges::end(r), std::move(comp), std::move(proj)
        );
    }
};

#endif  // GUAP_ALGO_COMB_SORT_H
