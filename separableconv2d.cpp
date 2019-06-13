#include "separableconv2d.hpp"

/**/

auto separableConv2d(Matrix<float, 3> const &input, Matrix<float, 3> const &dweights, Matrix<float, 2> const &pweights)
    -> Matrix<float, 3> {
    auto channels = input.size(0);

    if (channels != dweights.size(0) || channels != pweights.size(1)) {
        throw std::invalid_argument("Wrong channels args");
    }
    auto output = Matrix<float, 3>{pweights.size(0), input.size(1), input.size(2)};
    return output;
}

/**/

