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

int getItemIndex(const std::string& item, const std::string& filename){
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
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
    
    while(std::getline(file, line)){
        int index {0};
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
            if(cell == item){
                return index;
            }

            //std::cout << "[" << cell << "] size: " << cell.size()
            //<< " front: " << (int)cell.front()
            //<< " back: " << (int)cell.back() << std::endl;
            //std::cout<<cell<<' ';
            //std::cout<<item<<'\n';
            ++index;
        }
    }

    
    file.close();
    throw std::invalid_argument("item not found");

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