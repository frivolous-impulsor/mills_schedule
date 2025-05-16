#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <chrono>

std::vector<std::vector<std::string>> readCSV(const std::string& filename) {
    std::vector<std::vector<std::string>> data;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return data;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;

        while (std::getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    file.close();

    return data;
}

std::chrono::time_point<std::chrono::system_clock> parseDateTime(std::string dateTime, std::string format){
    std::tm tm = {};   
    std::stringstream ssStart(dateTime);
    size_t len {format.length()};
    char arr[len + 1];
    strcpy(arr, format.c_str());
    ssStart >> std::get_time(&tm, arr);
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}