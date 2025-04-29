template <typename T>
IndexPriorityQueue<T>::IndexPriorityQueue(bool isMax):
    m_isMax {isMax}
{}

template <typename T>
bool IndexPriorityQueue<T>::isMax(){
    return m_isMax;
}

template <typename T>
int IndexPriorityQueue<T>::getSize(){
    return m_size;
}

template <typename T>
bool IndexPriorityQueue<T>::empty(){
    return m_size < 1;
}

template <typename T>
double IndexPriorityQueue<T>::getValue(T content){
    if(m_content2index.find(content) == m_content2index.end()){
        throw std::invalid_argument("no such content in priority queue");
    }
    int key {m_content2index[content]};

    return (this->isMax())? m_values[key] : -m_values[key];
}

template <typename T>
void IndexPriorityQueue<T>::insert(T content, double value){
    if(m_content2index.find(content) != m_content2index.end()){
        this->update(content, value);
        return;
    }

    //check if content exists, if so, only update
    int queueSize {this->getSize()};
    int mapSize {static_cast<int>(m_inverseMap.size())};
    //when popped, queue size will decrement, 
    //but map size stay the same. node query will base on queue size
    if(queueSize < mapSize){
        //popped, newly inserted element will inherit the key of the element just beyond the queue size
        int key {m_inverseMap[queueSize]};
        m_values[key] = (this->isMax()) ? value : -value;
        m_content[key] = content;
        m_content2index[content] = key;


    }else{
        //add fresh element to queue
        if(this->isMax()){
            m_values.push_back(value);
        }else{
            m_values.push_back(-value);
        }
        m_content.push_back(content);
        m_content2index[content] = queueSize;
        m_inverseMap.push_back(queueSize);
        m_positionMap.push_back(queueSize);
    }
    m_size++;
    this->swim(queueSize);
}

template <typename T>
T IndexPriorityQueue<T>::peek(){
    if(this->empty()){
        throw std::underflow_error("priority queue empty, cannot peek");
    }

    int key {m_inverseMap[0]};
    return m_content[key];
}

template <typename T>
T IndexPriorityQueue<T>::pop(){
    if(this->empty()){
        throw std::underflow_error("priority queue empty, cannot pop");
    }

    int key {m_inverseMap[0]};
    T content{m_content[key]};

    int iLastItem {this->getSize() - 1};
    this->swap(0, iLastItem);
    m_content2index.erase(content);
    m_size--;

    if(!(this->empty())){
        this->sink(0);
    }

    return content;
}

template <typename T>
void IndexPriorityQueue<T>::update(T content, double newVal){

    if(m_content2index.find(content) == m_content2index.end()){
        return;
    }

    newVal = (this->isMax())? newVal : -newVal;

    int key {m_content2index[content]};
    double val {m_values[key]};
    m_values[key] = newVal;
    int position {m_positionMap[key]};
    if (val < newVal){
        this->swim(position);
    }else if(val > newVal){
        this->sink(position);
    }
}

template <typename T>
void IndexPriorityQueue<T>::increment(T content, double incrementation){
    if(m_content2index.find(content) == m_content2index.end()){
        return;
    }
    double val {this->getValue(content)};
    this->update(content, val + incrementation);
}


template <typename T>
void IndexPriorityQueue<T>::swap(int p1, int p2){
    m_positionMap[m_inverseMap[p1]] = p2;
    m_positionMap[m_inverseMap[p2]] = p1;
    int temp {m_inverseMap[p1]};
    m_inverseMap[p1] = m_inverseMap[p2];
    m_inverseMap[p2] = temp;
}

template <typename T>
void IndexPriorityQueue<T>::swim(int position){
    int key {m_inverseMap[position]};
    double val {m_values[key]};
    int parentPosition {(position -1)/2};
    while(parentPosition >= 0){
        int parentKey {m_inverseMap[parentPosition]};
        double parentVal {m_values[parentKey]};
        if(parentVal < val){
            this->swap(position, parentPosition);
            position = parentPosition;
            parentPosition = (position-1)/2;
        }else{
            break;
        }
    }
}

template <typename T>
void IndexPriorityQueue<T>::sink(int position){
    double val {m_values[m_inverseMap[position]]};

    while(1){
        int leftPosition {position*2 + 1};
        int rightPosition {position*2 + 2};
        if(leftPosition >= this->getSize()){
            break;
        }
        int biggerPosition {leftPosition};
        int leftKey {m_inverseMap[leftPosition]};
        double leftVal {m_values[leftKey]};
        if(rightPosition < this->getSize() && m_values[m_inverseMap[rightPosition]] > leftVal){
            biggerPosition = rightPosition;
        }
        double biggerVal {m_values[m_inverseMap[biggerPosition]]};
        if(val < biggerVal){
            swap(position, biggerPosition);
            position = biggerPosition;
        }else{
            break;
        }
    }
}

template <typename T>
bool IndexPriorityQueue<T>::inQueue(T content){
    return (m_content2index.find(content) != m_content2index.end());
}