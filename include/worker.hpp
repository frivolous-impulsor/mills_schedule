#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include "commonFunctions.hpp"


class Worker
{
private:
    std::string m_name {};
    int m_id {};
    double m_desiredHours {};
    double m_allocatedHours {};
    double m_allocatedPreferedHours {};
    std::vector<std::vector<bool>> m_preferenceMatrix {};

public:
    Worker(std::string name, double desiredHours, int id)
        :m_name {name}
        ,m_desiredHours {desiredHours}
        ,m_id {id}
        ,m_allocatedHours {0}
        {}

    std::string getName(){
        return m_name;
    }

    int getId(){
        return m_id;
    }

    void setDesiredHours(double hours){
        m_desiredHours = hours;
    }

    double getDesiredHours(){
        return m_desiredHours;
    }

    double getAllocatedHOurs(){
        return m_allocatedHours;
    }

    double getAllocatedPreferedHours(){
        return m_allocatedPreferedHours;
    }

    void allocatHours(double hours, bool isPrefered){
        if(hours >= 0){
            m_allocatedHours +=hours;
            if(isPrefered){
                m_allocatedPreferedHours +=hours;
            }
        }else{
            throw std::invalid_argument("cannot allocate negative hours");
        }
    }

    void setPreferenceMatrixCSV(std::string csvFileName){
        std::vector<std::vector<std::string>> result {readCSV(csvFileName)};
        if(result.size() == 0){
            throw std::invalid_argument("readCSV failed at ");
        }
        int r {0};
        for (auto row: result){
            for (auto val: row){
                if(m_preferenceMatrix.size() <= r){
                    m_preferenceMatrix.push_back({});
                }
                m_preferenceMatrix[r].push_back(std::stoi(val));

            }
            r++;
        }
    }

    std::vector<std::vector<bool>> getPreferenceMatrix(){
        return m_preferenceMatrix;
    }

};
