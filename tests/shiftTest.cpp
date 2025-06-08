#include "../include/catch_amalgamated.hpp"
#include "../include/shift.hpp"
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

    SECTION("commonfunction - get day of the week"){
        std::string s1 {"5/20/2025 8:30 PM"};
        std::string s2 {"5/22/2025 8:30 PM"};
        std::string timeFormat      {"%m/%d/%Y %H:%M %p"};

        tp tp1 {parseDateTime(s1, timeFormat)};
        tp tp2 {parseDateTime(s2, timeFormat)};
        int day1 {getDayOfWeek(tp1)};
        int day2 {getDayOfWeek(tp2)};
        REQUIRE(day1 == 2);
        REQUIRE(day2 == 4);
    }

    SECTION("shift initialization"){
        std::string timeString {"5/22/2024 8:30 PM"};
        std::string timeFormat      {"%m/%d/%Y %H:%M %p"};
        tp tp1 {parseDateTime(timeString, timeFormat)};
        std::string filename {"../../timeSheetSample.csv"};
        std::string manager {"Oliver Li"};
        stringMatrix mat {readCSV(filename)};

        Shift s {tp1};
        s.initializeShiftMatrix(mat, manager);
    }

    SECTION("fillStaff"){
        std::string timeString {"5/30/2025 8:30 PM"};
        std::string timeFormat      {"%m/%d/%Y %H:%M %p"};
        tp tp1 {parseDateTime(timeString, timeFormat)};
        std::string filename {"../../timeSheetShiftTest.csv"};
        std::string manager {"Oliver Li"};
        stringMatrix mat {readCSV(filename)};
        Shift s {tp1};
        s.initializeShiftMatrix(mat, manager);
        s.fillStaff(mat, manager);
        std::vector<int> numSlots {3, 4, 0, 4, 0, 0, 0};
        std::vector<std::vector<int>> numAvaliable {{1, 0, 0}, {0,1,0,2}, {}, {1,0,1,0}};
        for(int day {0}; day < s.getShiftMatrix().size(); ++day){
            REQUIRE(s.getShiftMatrix()[day].size() == numSlots[day]);

            for(int i {0}; i < s.getShiftMatrix()[day].size(); ++i){
                REQUIRE(s.getShiftMatrix()[day][i].getPeopleAvailable().size() == numAvaliable[day][i] );
                //std::cout<<s.getPeopleAvailable().size()<<" ";
            }
            //std::cout<<'\n';
        }
            
    }

    SECTION("density"){
                std::string timeString {"5/30/2025 8:30 PM"};
        std::string timeFormat      {"%m/%d/%Y %H:%M %p"};
        tp tp1 {parseDateTime(timeString, timeFormat)};
        std::string filename {"../../timeSheetShiftTest.csv"};
        std::string manager {"Oliver Li"};
        stringMatrix mat {readCSV(filename)};
        Shift s {tp1};
        s.initializeShiftMatrix(mat, manager);
        s.fillStaff(mat, manager);
        std::vector<int> numSlots {3, 4, 0, 4, 0, 0, 0};
        std::vector<std::vector<int>> numAvaliable {{1, 0, 0}, {0,1,0,2}, {}, {1,0,1,0}};
        
        s.setDensity();
        
    }

}