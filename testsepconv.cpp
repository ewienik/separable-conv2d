#define CATCH_CONFIG_MAIN

/**/

#include "catch.hpp"
#include "separableconv2d.hpp"

/**/

TEST_CASE("Compile simple case", "[compilation]") { separableConv2d({1, 1, 1}, {1, 1, 1}, {1, 1}); }

/**/

TEST_CASE("Wrong number of channels in args", "[args]") {
    REQUIRE_THROWS(separableConv2d({1, 1, 1}, {2, 1, 1}, {1, 1}));
    REQUIRE_THROWS(separableConv2d({1, 1, 1}, {1, 1, 1}, {1, 2}));
}

/**/

