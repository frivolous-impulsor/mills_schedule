#ifndef SHIFT_H
#define SHIFT_H

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include "commonFunctions.hpp"
#include "slot.hpp"
#include <unordered_map>

class Shift
{
private:
    std::vector<std::vector<Slot>> m_shiftMatrix {};
    std::unordered_map<std::string, int> m_nameID {};
    std::unordered_map<int, std::string> m_IDname {};
    int m_newId {0};

public:
    Shift(){};

    int addPerson(const std::string name){
        m_nameID.insert({name, m_newId});
        m_IDname.insert({m_newId, name});
        ++m_newId;
        return m_newId-1;
    }

    std::string getPerson(int ID){
        return m_IDname[ID];
    }

    void initializeShiftMatrix(const stringMatrix& mat, std::string& manager){
        while(m_shiftMatrix.size() < 7){
            std::vector<Slot> emptyDay {};
            m_shiftMatrix.push_back(emptyDay);
        }
        std::string createdBy {"Created By"};
        std::string start {"Start"};
        std::string end {"End"};
        std::string title {"Title"};
        std::string timeFormat      {"%m/%d/%Y %H:%M %p"};
        int personIndex {getItemIndex(createdBy, mat)};
        int startIndex {getItemIndex(start, mat)};
        int endIndex {getItemIndex(end, mat)};
        int titleIndex {getItemIndex(title, mat)};
        for(auto row: mat){
            if(row[personIndex] == manager){
                tp startTime { parseDateTime(row[startIndex], timeFormat )};
                tp endTime { parseDateTime(row[endIndex], timeFormat )};
                int dayI {getDayOfWeek(startTime)};
                int slotI {static_cast<int>(m_shiftMatrix[dayI].size())};
                Slot s {dayI, slotI, startTime, endTime};
                m_shiftMatrix[dayI].push_back(s);
            }else{
                this->addPerson(row[personIndex]);
            }
            
        }
    }

    void fillStaff(const stringMatrix& mat, std::string& manager){
        while(m_shiftMatrix.size() < 7){
            std::vector<Slot> emptyDay {};
            m_shiftMatrix.push_back(emptyDay);
        }
        std::string createdBy {"Created By"};
        std::string start {"Start"};
        std::string end {"End"};
        std::string title {"Title"};
        std::string timeFormat      {"%m/%d/%Y %H:%M %p"};
        int personIndex {getItemIndex(createdBy, mat)};
        int startIndex {getItemIndex(start, mat)};
        int endIndex {getItemIndex(end, mat)};
        int titleIndex {getItemIndex(title, mat)};
        for(auto row: mat){
            if(row[personIndex] != manager){
                tp startTime { parseDateTime(row[startIndex], timeFormat )};
                tp endTime { parseDateTime(row[endIndex], timeFormat )};
                int dayI {getDayOfWeek(startTime)};
                std::vector<tp> currentInterval {startTime, endTime};
                for(auto slot: this->getShiftMatrix()[dayI]){
                    if(isCovered(slot.getInterval(), currentInterval)){
                        slot.addAvailablePersonID(0);
                    }
                }
            }
        }
    }

    std::vector<std::vector<Slot>>& getShiftMatrix(){
        return m_shiftMatrix;
    }
};

#endif