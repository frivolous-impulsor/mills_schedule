#ifndef WORKER_H
#define WORKER_H

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

    double getHoursDiff(){
        try
        {
            double hoursDiff {m_desiredHours-m_allocatedHours};   //positive if desire more hours, negative if allocated for than needed
            return hoursDiff;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << "fail to set positive desired hours for some staffs before calculating hour difference between desired and allocated" << '\n';
            return 0;
        }
        
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

};

#endif