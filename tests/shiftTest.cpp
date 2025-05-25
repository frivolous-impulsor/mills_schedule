#include "../include/catch_amalgamated.hpp"
#include "../include/slot.hpp"
#include <stdexcept>
	

TEST_CASE("shift class", "[shift]"){
    SECTION("commonfunction - find item index"){
        std::string item {"Created By"};
        std::string filename {"../../timeSheetSample.csv"};
        auto mat {readCSV(filename)};
        int result {getItemIndex(item, mat)};
        REQUIRE(result == 2);

        item = "Title";
        result = getItemIndex(item, mat);
        REQUIRE(result == 3);

        item = "5/23/2025 3:30 PM";
        result = getItemIndex(item, mat);
        REQUIRE(result == 1);
    }

    SECTION("commonfunction - timeIntervalEnclosePercentage"){
        std::string t1 {"5/20/2025 9:00 PM"};
        std::string t2 {"5/20/2025 10:00 PM"};

        std::string separate1 {"5/20/2025 7:00 PM"};
        std::string separate2 {"5/20/2025 8:30 PM"};
        std::string timeFormat      {"%m/%d/%Y %H:%M %p"};

        tp tp1 {parseDateTime(t1, timeFormat)};
        tp tp2 {parseDateTime(t2, timeFormat)};
        tp tp3 {parseDateTime(separate1, timeFormat)};
        tp tp4 {parseDateTime(separate2, timeFormat)};
        std::vector<tp> intervalTarget {tp1, tp2};
        std::vector<tp> intervalCurrent {tp3, tp4};
        int intersectionIntervalPercentage {timeIntervalEnclosePersentage(intervalTarget, intervalCurrent)};
        REQUIRE(intersectionIntervalPercentage == 0);

        std::string enclose1 {"5/20/2025 9:00 PM"};
        std::string enclose2 {"5/20/2025 10:00 PM"};
        tp tp5 {parseDateTime(enclose1, timeFormat)};
        tp tp6 {parseDateTime(enclose2, timeFormat)};
        std::vector<tp> intervalCurrent1 {tp5, tp6};
        int intersectionIntervalPercentage1 {timeIntervalEnclosePersentage(intervalTarget, intervalCurrent1)};
        REQUIRE(intersectionIntervalPercentage1 == 100);

        std::string overlap1 {"5/20/2025 9:30 PM"};
        std::string overlap2 {"5/20/2025 10:30 PM"};
        tp tp7 {parseDateTime(overlap1, timeFormat)};
        tp tp8 {parseDateTime(overlap2, timeFormat)};
        std::vector<tp> intervalCurrent2 {tp7, tp8};
        int intersectionIntervalPercentage2 {timeIntervalEnclosePersentage(intervalTarget, intervalCurrent2)};
        REQUIRE(intersectionIntervalPercentage2 == 50);
        
    }

}