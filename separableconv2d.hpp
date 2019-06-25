#ifndef SEPARABLE_CONV2D_HPP
#define SEPARABLE_CONV2D_HPP

/**/

#include "matrix.hpp"

/**/

auto separableConv2d(Matrix<float, 3> const &input, Matrix<float, 3> const &dweights, Matrix<float, 2> const &pweights)
    -> Matrix<float, 3>;

/**/

#endif
