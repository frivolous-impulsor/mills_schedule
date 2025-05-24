#include "../include/catch_amalgamated.hpp"
#include "../include/slot.hpp"
#include <stdexcept>
	

TEST_CASE("shift class", "[shift]"){
    SECTION("commonfunction - find item index"){
        std::string item {"Created By"};
        std::string filename {"../../timeSheetSample.csv"};
        int result {getItemIndex(item, filename)};
        REQUIRE(result == 2);

        item = "Title";
        result = getItemIndex(item, filename);
        REQUIRE(result == 3);

        item = "5/23/2025 3:30 PM";
        result = getItemIndex(item, filename);
        REQUIRE(result == 1);
    }

}