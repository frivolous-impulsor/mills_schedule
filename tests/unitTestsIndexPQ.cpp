#include "../include/catch_amalgamated.hpp"
#include "../include/indexPriorityQueue.hpp"
#include <stdexcept>
	

TEST_CASE("index priority queue", "[Index PQ]"){
    SECTION("insert"){
        IndexPriorityQueue<int> q {};
        std::vector<int> ids {0,1,2,3,4};
        q.insert(ids[0], 2);
        q.insert(ids[1], 1);
        q.insert(ids[2], 3);
        q.insert(ids[3], 4);
        q.insert(ids[4], 8);
        std::vector<int> positionMap {2, 3, 4, 1, 0};
        std::vector<int> inverseMap {4, 3, 0, 1, 2};
        REQUIRE(q.getSize() == 5);
    }


    SECTION("peek top"){
        IndexPriorityQueue<int> q {};
        std::vector<int> ids {0,1,2,3,4};
        q.insert(ids[0], 2);
        q.insert(ids[1], 1);
        q.insert(ids[2], 10);
        q.insert(ids[3], 4);
        q.insert(ids[4], 8);


        REQUIRE(q.peek() == 2);
    }

    SECTION("pop top"){
        IndexPriorityQueue<int> q {};

        try{    //pop empty queue should throw exception
            q.pop();
            REQUIRE(false);
        }catch (std::underflow_error u){
            REQUIRE(true);
        }
        std::vector<int> ids {233,912,241,234,4};
        q.insert(ids[0], 4);
        q.insert(ids[1], 12);
        q.insert(ids[2], 100);
        q.insert(ids[3], 4.4);
        q.insert(ids[4], 8);

        REQUIRE(q.pop() == 241);
        REQUIRE(q.pop() == 912);
        REQUIRE(q.pop() == 4);
        REQUIRE(q.getSize() == 2);


    }

    SECTION("update"){
        IndexPriorityQueue<int> q {};
        std::vector<int> ids {421,43145,3414,22,45};
        q.insert(ids[0], 2);
        q.insert(ids[1], 1);
        q.insert(ids[2], 3);
        q.insert(ids[3], 221);
        q.insert(ids[4], 4);
        
        REQUIRE(q.peek() == 22);

        q.update(ids[3], 3);
        REQUIRE(q.peek() == 45);

    }

    SECTION("update minPQ"){
        IndexPriorityQueue<int> q {false};
        std::vector<int> ids {421,445,3414,22,45};
        q.insert(ids[0], 2);
        q.insert(ids[1], 1);
        q.insert(ids[2], 3);
        q.insert(ids[3], 221);
        q.insert(ids[4], 4);
        
        REQUIRE(q.peek() == 445);

        q.update(ids[1], 3);
        q.update(ids[4], -5);
        REQUIRE(q.peek() == 45);

    }

    SECTION("increment"){
        IndexPriorityQueue<int> q {};
        std::vector<int> ids {421,43145,3414,22,45};
        q.insert(ids[0], 2);
        q.insert(ids[1], 1);
        q.insert(ids[2], 3);
        q.insert(ids[3], 221);
        q.insert(ids[4], 4);
        
        q.increment(3414, 221);
        

        REQUIRE(q.peek() == 3414);
        REQUIRE(q.getValue(ids[2]) == 3+221);
    }

    SECTION("increment negative number"){
        IndexPriorityQueue<int> q {};
        std::vector<int> ids {421,43145,3414,22,45};
        q.insert(ids[0], 2);
        q.insert(ids[1], 1);
        q.insert(ids[2], 3);
        q.insert(ids[3], 221);
        q.insert(ids[4], 4);
        
        q.increment(ids[3], -220);
        

        REQUIRE(q.peek() == 45);
        REQUIRE(q.getValue(ids[3]) == 1);
    }

    SECTION("increment minPQ"){
        IndexPriorityQueue<int> q {false};
        std::vector<int> ids {421,315,3414,22,45};
        q.insert(ids[0], 10);
        q.insert(ids[1], 8);
        q.insert(ids[2], 9);
        q.insert(ids[3], 4);
        q.insert(ids[4], 5);
        
        REQUIRE(q.peek() == 22);
        q.increment(421, -8);
        REQUIRE(q.peek() == 421);

        q.increment(421, 3);
        REQUIRE(q.peek() == 22);

    }

    SECTION("getValue from content"){
        IndexPriorityQueue<int> q {};
        std::vector<int> ids {421,43145,3414,22,45};
        q.insert(ids[0], 2);
        q.insert(ids[1], 1);
        q.insert(ids[2], 3);
        q.insert(ids[3], 221);
        q.insert(ids[4], 4);
        
        REQUIRE(q.getValue(ids[2]) == 3);
        q.update(ids[2], 90);
        REQUIRE(q.getValue(ids[2]) == 90);

    }

    SECTION("insert duplicate item only update "){
        IndexPriorityQueue<int> q {};
        std::vector<int> ids {421,22,421};
        
        q.insert(ids[0], 60);
        q.insert(ids[1], 50);
        REQUIRE(q.getSize() == 2);
        q.insert(ids[2], 40);
        REQUIRE(q.getSize() == 2);
        REQUIRE(q.peek() == 22);
    }


    SECTION("integration test for insert, peek, pop, update"){
        IndexPriorityQueue<int> q {};
        std::vector<int> ids {4,12,1,55,53, 53, 99, 12, 16,12};

        q.insert(ids[0], 8);
        q.insert(ids[1], 2);
        q.insert(ids[2], 7.7);
        q.insert(ids[3], 3);
        q.insert(ids[4], 9);

        REQUIRE(q.peek() == 53);
        REQUIRE(q.pop() == 53);

        REQUIRE(q.peek() == 4);
        q.insert(ids[5], 9);
        REQUIRE(q.pop() == 53);
        q.insert(ids[6], 100);
        REQUIRE(q.pop() == 99);
        REQUIRE(q.pop() == 4);
        q.insert(ids[7], 5);
        q.insert(ids[8], 0);
        q.insert(ids[9],10);
        REQUIRE(q.pop() == 12);
        REQUIRE(q.pop() == 1);
        
    }

    SECTION("integration testing on min priority queue"){
        IndexPriorityQueue<int> q {false};
        std::vector<int> ids {1,2,10,11,9, 223, 223};

        q.insert(ids[0],8.1);
        q.insert(ids[1],6);
        q.insert(ids[2],3);
        q.insert(ids[3],3);
        q.insert(ids[4],111);
        q.insert(ids[5],90);
        REQUIRE(q.pop() == 10);
        REQUIRE(q.pop() == 11);
        q.insert(ids[6], 0);
        REQUIRE(q.pop() == 223);
        q.update(ids[4], 2);
        REQUIRE(q.pop() == 9);
    }

    SECTION("inQueue test"){
        IndexPriorityQueue<int> q {false};
        std::vector<int> ids {1,2,10,11,9, 223, 223};
        
        q.insert(ids[0],8.1);
        q.insert(ids[1],6);
        q.insert(ids[2],3);
        q.insert(ids[3],3);
        q.insert(ids[4],111);
        q.insert(ids[5],90);
        REQUIRE(q.inQueue(ids[3]));
        REQUIRE(q.pop() == 10);
        REQUIRE(q.pop() == 11);

        REQUIRE(!q.inQueue(ids[2]));
        REQUIRE(!q.inQueue(ids[3]));


        q.insert(ids[6], 0);
        REQUIRE(q.pop() == 223);
        q.update(ids[4], 2);

        REQUIRE(q.inQueue(ids[4]));
        REQUIRE(q.pop() == 9);
        REQUIRE(!q.inQueue(ids[4]));

    }

    SECTION("fuzz testing on queue"){
        //testing functions: insert(0), update(1), pop(2), peek(3)
        int iterations {100000};
        int whichFunct {0};
        IndexPriorityQueue<int> q {};
        int u {0};
        int v {0};
        for(int i {0}; i < iterations; ++i){
            whichFunct = std::rand() % 4;
            switch(whichFunct) {
                case 0:
                    //std::cout<<"insert case\n";
                    u = rand();
                    v = rand(); 
                    q.insert(u, v);
                    break;
                case 1:
                    //std::cout<<"update case\n";
                    u = rand();
                    v = rand(); 
                    q.update(u, v);
                    break;
                case 2:
                    //std::cout<<"pop case\n";

                    if(!q.empty()){
                        q.pop();
                    }
                    break;
                case 3:
                    //std::cout<<"peek case\n";

                    if(!q.empty()){
                        q.peek();
                    }
                    break;
                default:
                    break;
            }
        }
        REQUIRE(true);
    }

}