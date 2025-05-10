#include "../include/catch_amalgamated.hpp"
#include "../include/slot.hpp"
#include <stdexcept>
	

TEST_CASE("worker class basic functions", "[worker]"){
    SECTION("construction"){
        time_t start;
        time(&start);
        Slot s {0, 0, start, start};
        REQUIRE(true);
    }

    SECTION("duration calculation"){
        std::string startTimeString {"2023-06-17 12:36:00"};
        std::string endTimeString   {"2023-06-17 12:50:00"};
        std::string timeFormat      {"%Y-%m-%d %H:%M:%S"};
        time_t startTime {ParseDateTime(startTime, timeFormat)};
        time_t endTime {ParseDateTime(endTimeString, timeFormat)};
    }

}