#include "separableconv2d.hpp"

#include <tuple>
#include <vector>

/**/

using std::pair;
using std::vector;

/**/

namespace {

/**/

/**/

} // namespace

/**/

auto separableConv2d(Matrix<float, 3> const &input, Matrix<float, 3> const &dweights, Matrix<float, 2> const &pweights)
    -> Matrix<float, 3> {
    auto channels = input.size(0);

    if (channels != dweights.size(0) || channels != pweights.size(1)) {
        throw std::invalid_argument("Wrong channels args");
    }
    auto height = input.size(1);
    auto width = input.size(2);
    auto kernel_height = dweights.size(1);
    auto kernel_width = dweights.size(2);
    auto features = pweights.size(0);
    auto output = Matrix<float, 3>{features, height, width};

    auto kernelcx = kernel_width / 2;
    auto kernelcy = kernel_height / 2;
    auto padded = vector<float>(width + kernel_width - 1);

    for (auto c = 0; c < channels; c++) {
        for (auto h = 0; h < height; h++) {
            copy(input.begin(c, h), input.end(c, h), padded.begin() + kernelcx);
            // TODO: process padded with input & store result in output
        }
    }

    return output;
}

/**/

