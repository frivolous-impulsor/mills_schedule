#ifndef IPQ_H
#define IPQ_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <unordered_map>

template <typename T>
class IndexPriorityQueue{
public:
    IndexPriorityQueue(const bool isMax = true);
    bool isMax() const;
    int getSize() const;
    bool empty() const;
    double getValue(const T& content);
    void insert(const T& content, double value);
    T& peek();
    T& pop();
    void update(const T& content, double newVal);

    //regardless max or min priority queue, the value of content will be added with incrementation
    //that is, if incrementation is positive, value increase; incrementation is negative, value decrease
    void increment(const T& content, double incrementation);

    bool inQueue(const T& content);



private:
    bool m_isMax {true};
    int m_size {0};
    std::vector<double> m_values {};
    std::vector<const T*> m_content {};
    std::unordered_map<T, int> m_content2index {};
    std::vector<int> m_positionMap {};
    std::vector<int> m_inverseMap {};

    std::vector<int>& getPositionMap(){
        return m_positionMap;
    }
    std::vector<int>& getInverseMap(){
        return m_inverseMap;
    }

    void swap(int p1, int p2);

    void swim(int position);

    void sink(int position);


};

#include "../src/indexPriorityQueue.tpp"

#endif