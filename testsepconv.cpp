#define CATCH_CONFIG_MAIN

/**/

#include "catch.hpp"
#include "separableconv2d.hpp"

/**/

TEST_CASE("Compile simple case", "[compilation") { separableConv2d({1, 1, 1}, {1, 1, 1}, {1, 1}); }

/**/

