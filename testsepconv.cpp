#define CATCH_CONFIG_MAIN

/**/

#include "catch.hpp"
#include "separableconv2d.hpp"

#include <experimental/filesystem>

/**/

namespace fs = std::experimental::filesystem;

using std::ifstream;
using std::string;
using std::tuple;
using std::vector;

/**/

namespace {

/**/

constexpr char const *filename = "separable-conv2d-tests.txt";

/**/

auto readtestdata(string const &path) -> tuple<Matrix<float, 3>, Matrix<float, 3>, Matrix<float, 2>, Matrix<float, 3>> {
    auto width = -1;
    auto height = -1;
    auto channels = -1;
    auto kernel_width = -1;
    auto kernel_height = -1;
    auto features = -1;

    auto istrm = ifstream(path);
    REQUIRE(istrm.is_open());
    while (width < 1 || height < 1 || channels < 1 || kernel_width < 1 || kernel_height < 1 || features < 1) {
        auto name = string{};
        REQUIRE(istrm >> name);
        if (name == "width") {
            istrm >> width;
            continue;
        }
        if (name == "height") {
            istrm >> height;
            continue;
        }
        if (name == "channels") {
            istrm >> channels;
            continue;
        }
        if (name == "kernel_width") {
            istrm >> kernel_width;
            continue;
        }
        if (name == "kernel_height") {
            istrm >> kernel_height;
            continue;
        }
        if (name == "features") {
            istrm >> features;
            continue;
        }
        INFO("wrong syntax in " << path);
        REQUIRE(false);
    }
    auto input = Matrix<float, 3>{channels, height, width};
    auto dweights = Matrix<float, 3>{channels, kernel_width, kernel_height};
    auto pweights = Matrix<float, 2>{features, channels};
    auto output = Matrix<float, 3>{features, height, width};
    auto doneInput = false;
    auto doneDweights = false;
    auto donePweights = false;
    auto doneOutput = false;
    while (!doneInput || !doneDweights || !donePweights || !doneOutput) {
        auto name = string{};
        auto value = float{};
        REQUIRE(istrm >> name);
        if (name == "input") {
            for (auto c = 0; c < channels; c++) {
                for (auto h = 0; h < height; h++) {
                    for (auto w = 0; w < width; w++) {
                        REQUIRE(istrm >> value);
                        input.set(value, c, h, w);
                    }
                }
            }
            doneInput = true;
            continue;
        }
        if (name == "dweights") {
            for (auto c = 0; c < channels; c++) {
                for (auto h = 0; h < kernel_height; h++) {
                    for (auto w = 0; w < kernel_width; w++) {
                        REQUIRE(istrm >> value);
                        dweights.set(value, c, h, w);
                    }
                }
            }
            doneDweights = true;
            continue;
        }
        if (name == "pweights") {
            for (auto f = 0; f < features; f++) {
                for (auto c = 0; c < channels; c++) {
                    REQUIRE(istrm >> value);
                    pweights.set(value, f, c);
                }
            }
            donePweights = true;
            continue;
        }
        if (name == "output") {
            for (auto f = 0; f < features; f++) {
                for (auto h = 0; h < height; h++) {
                    for (auto w = 0; w < width; w++) {
                        REQUIRE(istrm >> value);
                        output.set(value, f, h, w);
                    }
                }
            }
            doneOutput = true;
            continue;
        }
        INFO("wrong syntax in " << path << " " << name);
        REQUIRE(false);
    }
    return {input, dweights, pweights, output};
}

/**/

} // namespace

/**/

TEST_CASE("Compile simple case") { separableConv2d({1, 1, 1}, {1, 1, 1}, {1, 1}); }

/**/

TEST_CASE("Wrong number of channels in args") {
    REQUIRE_THROWS(separableConv2d({1, 1, 1}, {2, 1, 1}, {1, 1}));
    REQUIRE_THROWS(separableConv2d({1, 1, 1}, {1, 1, 1}, {1, 2}));
}

/**/

TEST_CASE("Correct number of features") { REQUIRE(separableConv2d({1, 1, 1}, {1, 1, 1}, {3, 1}).size(0) == 3); }

/**/

TEST_CASE("Compare outputs") {
    static auto files = vector<string>{};
    static auto initialized = false;
    if (!initialized) {
        auto istrm = ifstream(filename);
        if (!istrm.is_open()) { return; }
        while (!istrm.eof()) {
            auto file = string{};
            istrm >> file;
            files.push_back(file);
        }
        initialized = true;
    }
    auto id = GENERATE(range(0, static_cast<int>(files.size())));
    auto path = files[id];
    DYNAMIC_SECTION("Compare #" << id << " with file " << fs::path(path).filename()) {
        REQUIRE(!path.empty());
        auto [input, dweights, pweights, expected] = readtestdata(path);
        auto output = separableConv2d(input, dweights, pweights);
        auto features = output.size(0);
        auto height = output.size(1);
        auto width = output.size(2);
        REQUIRE(features == expected.size(0));
        REQUIRE(height == expected.size(1));
        REQUIRE(width == expected.size(2));
        auto fails = 0;
        for (auto f = 0; f < features; f++) {
            for (auto h = 0; h < height; h++) {
                for (auto w = 0; w < width; w++) {
                    INFO("f, h, w: " << f << ", " << h << ", " << w);
                    CHECKED_ELSE(output.get(f, h, w) == Approx(expected.get(f, h, w))) { fails++; }
                    REQUIRE(fails < 10);
                }
            }
        }
    }
}

/**/

