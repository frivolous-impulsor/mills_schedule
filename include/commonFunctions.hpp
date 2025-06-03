#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <locale>

typedef std::chrono::time_point<std::chrono::system_clock> tp;
typedef std::vector<std::vector<std::string>> stringMatrix;

std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        ++start;
    }

    auto end = s.end();
    do {
        --end;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}



std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return data;
    }

    char bom[3];
    file.read(bom, 3);
    if (!(static_cast<unsigned char>(bom[0]) == 0xEF &&
        static_cast<unsigned char>(bom[1]) == 0xBB &&
        static_cast<unsigned char>(bom[2]) == 0xBF)) {
        // No BOM — rewind to start
        file.seekg(0);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            cell = trim(cell);
            if(cell.front() == '"'){
                cell.erase(0,1);
            }
            if(cell.back() == '"'){
                cell.erase(cell.length()-1,1);
            }
            cell = trim(cell);
            row.push_back(cell);
        }

        data.push_back(row);
    }

    file.close();
    return data;
}

int getItemIndex(const std::string& item, const std::vector<std::vector<std::string>>& matrix){
    int index {0};
    for(auto row: matrix){
        for(auto str: row){
            if(str == item){
                return index;
            }
            ++index;
        }
        index = 0;
    }
    throw std::invalid_argument("can't find item in matrix");
}

int timeIntervalEnclosePersentage(const std::vector<tp> targetInterval, const std::vector<tp> currentInterval){
    if (currentInterval[0]> targetInterval[1] || targetInterval[0] > currentInterval[1]){
        return 0;
    }
    tp intersectionStart { std::max(targetInterval[0], currentInterval[0])};
    tp intersectionEnd { std::min(targetInterval[1], currentInterval[1])};
    double targetDuration = (std::chrono::duration_cast<std::chrono::minutes>(targetInterval[1] - targetInterval[0])).count();
    double intersectionDuration =(std::chrono::duration_cast<std::chrono::minutes>(intersectionEnd - intersectionStart)).count();
    return static_cast<int>(intersectionDuration / targetDuration * 100);
}

bool isCovered(const std::vector<tp> targetInterval, const std::vector<tp> currentInterval){
    return timeIntervalEnclosePersentage(targetInterval, currentInterval) == 100;
}

int getDayOfWeek(tp dateTime){
    std::time_t t = std::chrono::system_clock::to_time_t(dateTime);

    // Convert time_t to tm structure (local time)
    std::tm* local_tm = std::localtime(&t);

    // Get day of the week (0 = Sunday, ..., 6 = Saturday)
    int weekday {local_tm->tm_wday};

    return weekday;
    
}


std::chrono::time_point<std::chrono::system_clock> parseDateTime(std::string& dateTime, std::string& format){
    std::tm tm = {};   
    std::stringstream ssStart(dateTime);
    size_t len {format.length()};
    char arr[len + 1];
    strcpy(arr, format.c_str());
    ssStart >> std::get_time(&tm, arr);
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

#endif