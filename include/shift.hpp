#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include "commonFunctions.hpp"
#include "slot.hpp"

class Shift
{
private:
    std::vector<std::vector<Slot>> m_shiftMatrix {};

public:
    Shift(){};
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
            }
        }
    }

    std::vector<std::vector<Slot>> getShiftMatrix(){
        return m_shiftMatrix;
    }
};

