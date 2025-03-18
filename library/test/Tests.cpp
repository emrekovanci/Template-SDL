#include <catch2/catch_test_macros.hpp>

#include <Core/Lib.hpp>

TEST_CASE("some_fun")
{
    Lib lib;
    REQUIRE(lib.getName() == "Library");
}
