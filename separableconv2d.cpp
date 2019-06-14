#include "separableconv2d.hpp"

#include <tuple>
#include <vector>

#include <iostream>

/**/

// using std::pair;
using std::max;
using std::min;
using std::vector;

/**/

namespace {

/**/

void compose(float *output, float const *padded, int pwidth, float const *dweight, int dwidth, float pweight) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    for (auto p = 0; p < pwidth - dwidth + 1; p++, padded++, output++) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        for (auto d = 0; d < dwidth; d++) { *output += padded[d] * dweight[d] * pweight; }
    }
}

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
    auto kernelHeight = dweights.size(1);
    auto kernelWidth = dweights.size(2);
    auto features = pweights.size(0);
    auto output = Matrix<float, 3>{features, height, width};

    auto kernelcx = kernelWidth / 2;
    auto kernelcy = kernelHeight / 2;
    auto padded = vector<float>(width + kernelWidth - 1);

    std::cout << "setup: " << height << " " << width << " " << kernelHeight << " " << kernelWidth << " " << features
              << " " << kernelcx << " " << kernelcy << " " << padded.size() << "\n";
    for (auto c = 0; c < channels; c++) {
        for (auto h = 0; h < height; h++) {
            copy(input.begin(c, h), input.end(c, h), padded.begin() + kernelcx);
            auto kbegin = max(0, kernelcy - h);
            auto kheight = min(kernelHeight, height - h + 1 - kernelcy);
            std::cout << "check: " << c << " " << h << " " << kbegin << " " << kheight << "\n";
            for (auto kh = kbegin; kh < kheight; kh++) {
                for (auto f = 0; f < features; f++) {
                    std::cout << "compose: " << kh << " " << f << "\n";
                    compose(output.begin(f, h), &padded.front(), padded.size(), dweights.begin(c, kh), dweights.size(1),
                            pweights.get(f, c));
                }
            }
        }
    }

    return output;
}

/**/
