#include "../include/catch_amalgamated.hpp"
#include "../include/schedule.hpp"

TEST_CASE("main", "[main]"){
    SECTION("null"){
        std::string timeString {"6/15/2025 1:30 AM"};
        std::string timeFormat      {"%m/%d/%Y %H:%M %p"};
        tp tp1 {parseDateTime(timeString, timeFormat)};
        std::string filename {"../../timeSheetFinalTest.csv"};
        std::string manager {"Oliver Li"};
        stringMatrix mat {readCSV(filename)};
        Shift s {tp1};
        s.initializeShiftMatrix(mat, manager);
        s.fillStaff(mat, manager);
        Schedule schedule {};
        schedule.arrange(s);
        schedule.printResult(s);
        
    }

}