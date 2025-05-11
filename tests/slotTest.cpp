#include "../include/catch_amalgamated.hpp"
#include "../include/slot.hpp"
#include <stdexcept>
	

TEST_CASE("worker class basic functions", "[worker]"){
    SECTION("construction"){
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        Slot s {0, 0, now, now};
        REQUIRE(true);
    }

    
    SECTION("duration calculation"){
        std::string startTimeString {"2023-06-17 12:36:00"};
        std::string endTimeString   {"2023-06-18 12:50:00"};
        std::string timeFormat      {"%Y-%m-%d %H:%M:%S"};

        std::tm tmStart = {};
        std::stringstream ssStart("2023-06-17 12:36:00");
        ssStart >> std::get_time(&tmStart, "%Y-%m-%d %H:%M:%S");
        auto tpStart = std::chrono::system_clock::from_time_t(std::mktime(&tmStart));

        std::tm tmEnd = {};
        std::stringstream ssEnd("2023-06-18 12:50:00");
        ssEnd >> std::get_time(&tmEnd, "%Y-%m-%d %H:%M:%S");
        auto tpEnd = std::chrono::system_clock::from_time_t(std::mktime(&tmEnd));

        Slot s {0, 0, tpStart, tpEnd};
        REQUIRE(s.getDurationInMinute() == 14 + 24*60);
    }
    
    SECTION("set num people wanted"){
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        Slot s {0, 0, now, now};
        s.setNumPeopleWanted(5);
        s.setNumPeopleWanted(3);
        s.setNumPeopleWanted(7);
        REQUIRE(s.getNumPeopleWanted() == 7);
        try
        {
            s.setNumPeopleWanted(-1);
            REQUIRE(false);
        }
        catch(const std::invalid_argument& e)
        {
            REQUIRE(true);
        }
        
    }

    SECTION("assign id"){
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        Slot s {0, 0, now, now};
        REQUIRE(s.getNumPeopleWanted() == 0);
        s.setNumPeopleWanted(3);
        REQUIRE(s.getNumPeopleWanted() == 3);
        REQUIRE(s.getNumAssigned() == 0);
        s.assignPersonID(4);
        REQUIRE(s.getNumAssigned() == 1);
        REQUIRE(s.wantMorePeople());
        s.assignPersonID(4);
        REQUIRE(s.getNumAssigned() == 1);
        REQUIRE(s.wantMorePeople());
        s.assignPersonID(5);
        s.assignPersonID(0);
        REQUIRE(s.getNumAssigned() == 3);
        REQUIRE(!s.wantMorePeople());
    }


}