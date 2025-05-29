#include "../include/catch_amalgamated.hpp"
#include "../include/slot.hpp"
#include <stdexcept>
	

TEST_CASE("worker class basic functions", "[worker]"){
    SECTION("construction"){
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        Slot s {0, 0, now, now};
        REQUIRE(true);
    }

    SECTION("parse time calc"){
        std::string startTimeString {"5/20/2025 9:00 PM"};
        std::string endTimeString {"5/20/2025 9:10 PM"};
        std::string timeFormat      {"%m/%d/%Y %H:%M %p"};
        std::chrono::time_point<std::chrono::system_clock> tpStart {parseDateTime(startTimeString, timeFormat)};
        std::chrono::time_point<std::chrono::system_clock> tpEnd {parseDateTime(endTimeString, timeFormat)};

        auto timeDiffDurationMin1 = std::chrono::duration_cast<std::chrono::minutes>(tpEnd - tpStart);
        REQUIRE(timeDiffDurationMin1.count() == 10);

        auto timeDiffDurationMin2 = std::chrono::duration_cast<std::chrono::minutes>(tpStart - tpEnd);
        REQUIRE(timeDiffDurationMin2.count() == -10);

        endTimeString = "5/21/2025 11:00 AM";
        tpEnd = parseDateTime(endTimeString, timeFormat);
        
        timeDiffDurationMin2 = std::chrono::duration_cast<std::chrono::minutes>(tpEnd - tpStart);
        REQUIRE(timeDiffDurationMin2.count() == 14*60);

    }
    
    SECTION("duration calculation"){
        std::string startTimeString {"2023-06-17 9:30:00"};
        std::string endTimeString   {"2023-06-18 10:00:00"};
        std::string timeFormat      {"%Y-%m-%d %H:%M:%S"};

        std::chrono::time_point<std::chrono::system_clock> tpStart {parseDateTime(startTimeString, timeFormat)};
        std::chrono::time_point<std::chrono::system_clock> tpEnd {parseDateTime(endTimeString, timeFormat)};


        Slot s {0, 0, tpStart, tpEnd};
        REQUIRE(s.getDurationInMinute() == 30 + 24*60);
        REQUIRE(s.getDurationInHour() == (30+24*60)/60.0);
    }

    SECTION("invalid construction"){
        std::string startTimeString {"2023-06-17 9:30:00"};
        std::string endTimeString   {"2023-06-17 10:00:00"};
        std::string timeFormat      {"%Y-%m-%d %H:%M:%S"};

        std::chrono::time_point<std::chrono::system_clock> tpStart {parseDateTime(startTimeString, timeFormat)};
        std::chrono::time_point<std::chrono::system_clock> tpEnd {parseDateTime(endTimeString, timeFormat)};
        try
        {
            Slot s {0, 0, tpEnd, tpStart};
            REQUIRE(false);
        }
        catch(const std::invalid_argument& e)
        {   
            REQUIRE(true);
        }
        
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
        REQUIRE(s.wantMorePeople());
        s.addAvailablePersonID(0);
        s.addAvailablePersonID(1);
        s.addAvailablePersonID(2);
        s.addAvailablePersonID(3);
        s.assignPersonID(0);
        s.assignPersonID(1);
        s.assignPersonID(2);
        s.assignPersonID(3);
        REQUIRE(s.wantMorePeople());
        s.setNumPeopleWanted(3);
        REQUIRE(!s.wantMorePeople());
        REQUIRE(s.getNumAssigned() == 4);
    }

    SECTION("assign id"){
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        Slot s {0, 0, now, now};
        REQUIRE(s.getNumPeopleWanted() == 0);
        s.setNumPeopleWanted(3);
        REQUIRE(s.getNumPeopleWanted() == 3);
        REQUIRE(s.getNumAssigned() == 0);
        s.assignPersonID(4);
        REQUIRE(s.getNumAssigned() == 0);

        REQUIRE(s.wantMorePeople());
        s.addAvailablePersonID(4);
        s.assignPersonID(4);
        REQUIRE(s.getNumAssigned() == 1);
        REQUIRE(s.wantMorePeople());
        s.addAvailablePersonID(5);
        s.addAvailablePersonID(0);
        s.assignPersonID(5);
        REQUIRE(s.getNumAssigned() == 2);
        s.assignPersonID(0);
        REQUIRE(s.getNumAssigned() == 3);

        REQUIRE(!s.wantMorePeople());
    }

}