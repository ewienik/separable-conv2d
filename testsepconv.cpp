#define CATCH_CONFIG_MAIN

/**/

#include "catch.hpp"
#include "separableconv2d.hpp"

/**/

using std::ifstream;
using std::string;
using std::vector;

/**/

namespace {

    /**/

    constexpr char const * filename = "separable-conv2d-tests.txt";

    /**/

}

/**/

TEST_CASE("Compile simple case") { separableConv2d({1, 1, 1}, {1, 1, 1}, {1, 1}); }

/**/

TEST_CASE("Wrong number of channels in args") {
    REQUIRE_THROWS(separableConv2d({1, 1, 1}, {2, 1, 1}, {1, 1}));
    REQUIRE_THROWS(separableConv2d({1, 1, 1}, {1, 1, 1}, {1, 2}));
}

/**/

TEST_CASE("Correct number of features") {
    REQUIRE(separableConv2d({1, 1, 1}, {1, 1, 1}, {3, 1}).size(0) == 3);
}

/**/

TEST_CASE("Compare outputs") {
    static auto files = vector<string>{};
    static auto initialized = false;
    if (!initialized) {
        auto istrm = ifstream(filename);
        REQUIRE(istrm.is_open());
        while (!istrm.eof()) {
            auto file = string{};
            istrm >> file;
            files.push_back(file);
        }
        initialized = true;
    }
    SECTION("Compare with files") {
        auto id = GENERATE(range(0, static_cast<int>(files.size())));
        auto path = files[id];
        REQUIRE(!path.empty());
    }
}

/**/

