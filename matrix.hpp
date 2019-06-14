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

    int size(int dim) const {
        if (dim < 0 || dim >= Rank) { return -1; }
        return dims_[dim];
    }

    /**/

    template <typename... Idx>
    Type get(Idx... args) const {
        return data_[offset(args...)];
    }

    /**/

    template <typename... Idx>
    void set(Type value, Idx... args) {
        data_[offset(args...)] = value;
    }

    /**/

    template <typename... Idx>
    Type const *begin(Idx... args) const {
        static_assert(Rank - 1 == sizeof...(Idx), "There should be index for all - 1 dimensions");
        return &data_[offset(args..., 0)];
    }

    /**/

    template <typename... Idx>
    Type const *end(Idx... args) const {
        return begin(args...) + dims_.last();
    }

    /**/

private:
    std::array<int, Rank> dims_;
    std::vector<Type> data_;

    /**/

    template <typename... Idx>
    auto offset(Idx... args) const {
        static_assert(Rank == sizeof...(Idx), "There should be index for all dimensions");
        auto idxs = std::array<int, Rank>{args...};
        auto offset = int{};
        for (auto id = 0; id < Rank; id++) {
            auto idx = idxs[id];
            auto size = dims_[id];
            if (idx < 0 || idx >= size) { throw std::out_of_range("Indexes out of scope"); }
            offset = offset * size + idx;
        }
        return offset;
    }
};

/**/

#endif
