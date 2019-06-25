#ifndef MATRIX_HPP
#define MATRIX_HPP

/**/

#include <algorithm>
#include <array>
#include <numeric>
#include <stdexcept>
#include <vector>

/**/

template <typename Type, int Rank>
struct Matrix {
    /**/

    template <typename... Dim>
    Matrix(Dim... args) {
        static_assert(Rank > 0, "Rank should be positive");
        static_assert(Rank == sizeof...(Dim), "All dimensions should be constructed");
        dims_ = {args...};
        if (*std::min_element(dims_.begin(), dims_.end()) < 1) {
            throw std::out_of_range("All dimensions should be positive");
        }
        data_.resize(std::accumulate(dims_.begin(), dims_.end(), 1, std::multiplies<int>()));
    }

    /**/

    [[nodiscard]] auto size(int dim) const -> int {
        if (dim < 0 || dim >= Rank) { return -1; }
        return dims_[dim]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    /**/

    template <typename... Idx>
    [[nodiscard]] auto get(Idx... args) const -> Type {
        return data_[offset(args...)];
    }

    /**/

    template <typename... Idx>
    void set(Type value, Idx... args) {
        data_[offset(args...)] = value;
    }

    /**/

    template <typename... Idx>
    auto begin(Idx... args) -> Type * {
        static_assert(Rank - 1 == sizeof...(Idx), "There should be index for all - 1 dimensions");
        return &data_[offset(args..., 0)];
    }
    template <typename... Idx>
    [[nodiscard]] auto begin(Idx... args) const -> Type const * {
        static_assert(Rank - 1 == sizeof...(Idx), "There should be index for all - 1 dimensions");
        return &data_[offset(args..., 0)];
    }

    /**/

    template <typename... Idx>
    auto end(Idx... args) -> Type * {
        return begin(args...) + dims_.back();
    }
    template <typename... Idx>
    [[nodiscard]] auto end(Idx... args) const -> Type const * {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        return this->begin(args...) + dims_.back();
    }

    /**/

private:
    std::array<int, Rank> dims_;
    std::vector<Type> data_;

    /**/

    template <typename... Idx>
    [[nodiscard]] auto offset(Idx... args) const {
        static_assert(Rank == sizeof...(Idx), "There should be index for all dimensions");
        auto idxs = std::array<int, Rank>{args...};
        auto offset = int{};
        for (auto id = 0; id < Rank; id++) {
            auto idx = idxs[id];   // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
            auto size = dims_[id]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
            if (idx < 0 || idx >= size) { throw std::out_of_range("Indexes out of scope"); }
            offset = offset * size + idx;
        }
        return offset;
    }
};

/**/

#endif
