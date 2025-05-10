#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <set>
#include <chrono>
#include "commonFunctions.hpp"

class Slot{
private:
    int m_dayIndex {};
    int m_slotIndex {};
    time_t m_startTime {};
    time_t m_endTime {};
    double m_duration {};     //duration in minutes
    int m_peopleWanted {0};
    bool m_critical {true};
    std::set<int> m_peopleAssigned {};
public:
    Slot(int dayI, int slotI, time_t startT, time_t endT)
        :m_dayIndex {dayI}
        ,m_slotIndex {slotI}
        ,m_startTime {startT}
        ,m_endTime {endT}
        {
            double timeDiffSec = difftime(endT, startT);
            m_duration = timeDiffSec/60;
        }
};