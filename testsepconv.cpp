#define CATCH_CONFIG_MAIN

/**/

#include "catch.hpp"
#include "separableconv2d.hpp"

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

