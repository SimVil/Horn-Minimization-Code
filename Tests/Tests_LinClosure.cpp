
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "../catch.h"
#include "../CanonicalBasis/fca_linclosure.h"
#include <vector>



TEST_CASE("FCA::LinClosure::Apply: Simple tests")
{
    std::vector<FCA::Attribute> sigma = {"a", "b", "c", "d", "e"};

    SECTION("Check for correct closure")
}

