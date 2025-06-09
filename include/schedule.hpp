#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "../include/slot.hpp"
#include "../include/worker.hpp"
#include "../include/shift.hpp"
#include "../include/indexPriorityQueue.hpp"

class Schedule{
private:
    IndexPriorityQueue<Slot> m_densityQueue {false};

public:
    Schedule()
    {
    };
    
    void setDensity(Shift& shift){
        for(int i {0}; i < shift.getShiftMatrix().size(); ++i){
            for(int j {0}; j < shift.getShiftMatrix()[i].size(); ++j){
                int numAvailable {static_cast<int>(shift.getShiftMatrix()[i][j].getPeopleAvailable().size())};
                int dragWeight {};
                if(shift.getShiftMatrix()[i][j].isCritical()){
                    dragWeight = 0;
                }else{
                    dragWeight = rand()%9 + 1;
                }
                
                m_densityQueue.insert(shift.getShiftMatrix()[i][j], numAvailable+dragWeight);
            }
        }
        while(!m_densityQueue.empty()){
            std::cout<<m_densityQueue.pop().getPeopleAvailable().size()<<" ";
        }
    }

    
    void schedule(){

    }
    
};





#endif