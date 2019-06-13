#ifndef SEPARABLE_CONV2D_HPP
#define SEPARABLE_CONV2D_HPP

/**/

#include "matrix.hpp"

/**/

Matrix<float, 3> separableConv2d(Matrix<float, 3> const &input, Matrix<float, 3> const &dweights,
                                 Matrix<float, 2> const &pweights);

/**/

#endif
