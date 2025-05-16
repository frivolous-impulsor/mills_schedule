#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <stdexcept>
#include <set>
#include <chrono>
#include "commonFunctions.hpp"

class Slot{
private:
    int m_dayIndex {};
    int m_slotIndex {};
    std::chrono::time_point<std::chrono::system_clock> m_startTime {};
    std::chrono::time_point<std::chrono::system_clock> m_endTime {};
    double m_duration {};     //duration in minutes
    int m_numPeopleWanted {0};
    bool m_critical {true};
    std::set<int> m_peopleAssignedID {};
public:
    Slot(int dayI, int slotI,   std::chrono::time_point<std::chrono::system_clock> startT, 
                                std::chrono::time_point<std::chrono::system_clock> endT)
        :m_dayIndex {dayI}
        ,m_slotIndex {slotI}
        ,m_startTime {startT}
        ,m_endTime {endT}
        {
            auto timeDiffSec = std::chrono::duration_cast<std::chrono::minutes>(endT - startT);
            m_duration = timeDiffSec.count();
        }

    double getDurationInMinute(){
        return m_duration;
    }

    double getDurationInHour(){
        return m_duration/60;
    }

    void setNumPeopleWanted(int n){
        if(n<0){
            throw std::invalid_argument("only non-negative number of people can be wanted");
        }
        m_numPeopleWanted = n;
    }

    void assignPersonID(int id){
        m_peopleAssignedID.insert(id);
    }

    int getNumPeopleWanted(){
        return m_numPeopleWanted;
    }


    std::set<int> getPeopleAssigned(){
        return m_peopleAssignedID;
    }

    int getNumAssigned(){
        return m_peopleAssignedID.size();
    }

    bool wantMorePeople(){
        return this->getNumAssigned() < m_numPeopleWanted;
    }

};