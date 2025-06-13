#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "../include/slot.hpp"
#include "../include/worker.hpp"
#include "../include/shift.hpp"
#include "../include/indexPriorityQueue.hpp"

class Schedule{
private:
    IndexPriorityQueue<Slot> m_slotQueue {false};

public:
    Schedule()
    {srand(time(0));};
    
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
                
                m_slotQueue.insert(shift.getShiftMatrix()[i][j], numAvailable+dragWeight);
            }
        }
    }

    
    void schedule(Shift& shift){
        std::vector<std::set<int>> workedIDInDays {};
        for(int i {0}; i < 7; ++i){
            workedIDInDays.push_back({});
        }
        while(!m_slotQueue.empty()){

            Slot& currentSlot {m_slotQueue.peek()};
            if(currentSlot.getNumPeopleAvailable() < 1){
                //std::cout<<"popped\n";
                m_slotQueue.pop();
                continue;
            }

            const int dayIndex {currentSlot.getIndex()[0]};

            //randomized weight attached to each criterion, hoursDiff weights more often heavier than others
            const int preferWeight {rand()%3 + 1};
            const int hoursDiffWeight {rand()%10 + 5};
            const int workedTodayWeight {rand()%3 + 1};

            //find the best fit staff among all available staff 
            //according to (hours deviance, worked that day, preference)
            IndexPriorityQueue<int> properStaff {};    //max pq determine which staff gets the slot first
            for(std::tuple<int, bool> id_pref: currentSlot.getPeopleAvailable()){
                int id {std::get<0>(id_pref)};
                const int prefer = (std::get<1>(id_pref))? 1 : 0;
                const double hoursDiff {shift.getStaffs()[ id ].getHoursDiff() };
                const int workedToday = (workedIDInDays[dayIndex].find(id) != workedIDInDays[dayIndex].end())? 1 : 0;
                
                //each criterion is weighted. Higher hoursDiff, is preferred should have higher probability of getting picked
                //whereas if worked that day already, then lower probability
                const double score {hoursDiff * hoursDiffWeight + prefer * preferWeight - workedToday * workedTodayWeight };

                properStaff.insert(id, score);
                
            }
            const int selectedID {properStaff.pop()};
            //std::cout<<currentSlot.getNumPeopleAvailable()<<"\n";
            currentSlot.assignPersonID(selectedID);
            //std::cout<<currentSlot.getNumAssigned();
            //std::cout<<currentSlot.getNumPeopleAvailable()<<"\n";
            m_slotQueue.increment(currentSlot, -1);


            
        
        }

        
    }
    
    void printResult(Shift& shift){
        for(auto& row: shift.getShiftMatrix()){
            for(Slot& slot: row){
                std::cout<<"[";
                for(int id: slot.getPeopleAssigned()){
                    std::cout<< shift.getPerson(id) <<",";
                }
                std::cout<<"]";
            }
            std::cout<<'\n';
        }
    }
};





#endif