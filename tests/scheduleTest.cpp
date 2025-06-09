#include "../include/catch_amalgamated.hpp"
#include "../include/schedule.hpp"

TEST_CASE("main", "[main]"){
    SECTION("null"){
        std::string timeString {"5/30/2025 8:30 PM"};
        std::string timeFormat      {"%m/%d/%Y %H:%M %p"};
        tp tp1 {parseDateTime(timeString, timeFormat)};
        std::string filename {"../../timeSheetShiftTest.csv"};
        std::string manager {"Oliver Li"};
        stringMatrix mat {readCSV(filename)};
        Shift s {tp1};
        s.initializeShiftMatrix(mat, manager);
        s.fillStaff(mat, manager);
        Schedule schedule {};
        schedule.setDensity(s);
        schedule.schedule();
        
    }
}