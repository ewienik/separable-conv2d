cmake_minimum_required(VERSION 3.14)

project(separable-conv2d)

add_executable(testsepconv separableconv2d.cpp testsepconv.cpp)

file(GLOB TEST_DATA "testdata/*")
string(REPLACE ";" "\n" TEST_DATA_LINES "${TEST_DATA}")
file(WRITE ${PROJECT_BINARY_DIR}/separable-conv2d-tests.txt ${TEST_DATA_LINES})

set_target_properties(testsepconv PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_CLANG_TIDY "clang-tidy;-header-filter=\".*\";-checks=\",-*,clang*,bugprone*,cppcoreguidelines*,-cppcoreguidelines-avoid-magic-numbers,misc*,modernize*,performance*,-readability-magic-numbers,readability*\""
)

