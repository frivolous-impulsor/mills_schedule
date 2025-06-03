#include "../include/slot.hpp"
#include "../include/worker.hpp"
#include "../include/shift.hpp"
#include "../include/indexPriorityQueue.hpp"

void schedule(){
    std::string filename {"../../timeSheetSample.csv"};
    std::string manager {"Oliver Li"};
    stringMatrix readings {readCSV(filename)};

    Shift shift {};
    shift.initializeShiftMatrix(readings, manager);
    
    shift.fillStaff(readings, manager);
}