#ifndef SLOT_H
#define SLOT_H

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
    std::set<std::vector<int>> m_peopleAvailableID {};
public:
    Slot(int dayI, int slotI,   std::chrono::time_point<std::chrono::system_clock> startT, 
                                std::chrono::time_point<std::chrono::system_clock> endT)
        :m_dayIndex {dayI}
        ,m_slotIndex {slotI}
        ,m_startTime {startT}
        ,m_endTime {endT}
        {
            auto timeDiffDurationMin = std::chrono::duration_cast<std::chrono::minutes>(endT - startT);
            m_duration = timeDiffDurationMin.count();
            if (m_duration < 0){
                throw std::invalid_argument("negative duration detected, end time shoud be no earlier than start time");
            }
        }

    double getDurationInMinute(){
        return m_duration;
    }

    std::vector<int> getIndex() const{
        std::vector<int> index {m_dayIndex, m_slotIndex};
        return index;
    }

    void cancelCritical(){
        m_critical = false;
    }

    bool isCritical(){
        return m_critical;
    }

    double getDurationInHour(){
        return m_duration/60;
    }

    std::vector<tp> getInterval(){
        std::vector<tp> interval {m_startTime, m_endTime};
        return interval;
    }

    void setNumPeopleWanted(int n){
        if(n<0){
            throw std::invalid_argument("only non-negative number of people can be wanted");
        }
        m_numPeopleWanted = n;
    }

    void addAvailablePersonID(int id, int preference){
        //std::cout<<"["<<id<<"] ";
        m_peopleAvailableID.insert({id, preference});
        //std::cout<<"size: "<<m_peopleAvailableID.size()<<'\n';
    }

    void addAvailablePersonID(int id){
        //std::cout<<"["<<id<<"] ";
        m_peopleAvailableID.insert({id, 0});
        //std::cout<<"size: "<<m_peopleAvailableID.size()<<'\n';
    }

    std::set<std::vector<int>> getPeopleAvailable(){
        return m_peopleAvailableID;
    }

    void assignPersonID(int id){
        if(m_peopleAvailableID.find({id, 0}) != m_peopleAvailableID.end() || m_peopleAvailableID.find({id, 1}) != m_peopleAvailableID.end()){
            m_peopleAssignedID.insert(id);
            m_peopleAvailableID.erase({id,0});
            m_peopleAvailableID.erase({id,1});
        }
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

    bool operator<(const Slot& other) const{
        return m_dayIndex < other.getIndex()[0] && m_slotIndex < other.getIndex()[1];
    };

    bool operator==(const Slot& other) const{
        return m_dayIndex == other.getIndex()[0] && m_slotIndex == other.getIndex()[1];
    };

};

template <>
struct std::hash<Slot>
{
  std::size_t operator()(const Slot& s) const
  {
    using std::size_t;
    using std::hash;

    // Compute individual hash values for first,
    // second and third and combine them using XOR
    // and bit shifting:

    return ((hash<int>()(s.getIndex()[0])
             ^ (hash<int>()(s.getIndex()[1]) << 1)) >> 1);
  }
};

#endif