#include "../include/catch_amalgamated.hpp"
#include "../include/indexPriorityQueue.hpp"
#include <stdexcept>
	

TEST_CASE("index priority queue", "[Index PQ]"){
    SECTION("insert"){
        IndexPriorityQueue<int> q {};
        q.insert(0, 2);
        q.insert(1, 1);
        q.insert(2, 3);
        q.insert(3, 4);
        q.insert(4, 8);
        std::vector<int> positionMap {2, 3, 4, 1, 0};
        std::vector<int> inverseMap {4, 3, 0, 1, 2};
        REQUIRE(q.getSize() == 5);
    }


    SECTION("peek top"){
        IndexPriorityQueue<int> q {};
        q.insert(0, 2);
        q.insert(1, 1);
        q.insert(2, 10);
        q.insert(3, 4);
        q.insert(4, 8);

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

        q.insert(233, 4);
        q.insert(912, 12);
        q.insert(241, 100);
        q.insert(234, 4.4);
        q.insert(4, 8);

        REQUIRE(q.pop() == 241);
        REQUIRE(q.pop() == 912);
        REQUIRE(q.pop() == 4);
        REQUIRE(q.getSize() == 2);


    }

    SECTION("update"){
        IndexPriorityQueue<int> q {};
        q.insert(421, 2);
        q.insert(43145, 1);
        q.insert(3414, 3);
        q.insert(22, 221);
        q.insert(45, 4);
        
        REQUIRE(q.peek() == 22);

        q.update(22, 3);
        REQUIRE(q.peek() == 45);

    }

    SECTION("update minPQ"){
        IndexPriorityQueue<int> q {false};
        q.insert(421, 2);
        q.insert(445, 1);
        q.insert(3414, 3);
        q.insert(22, 221);
        q.insert(45, 4);
        
        REQUIRE(q.peek() == 445);

        q.update(445, 3);
        q.update(45, -5);
        REQUIRE(q.peek() == 45);

    }

    SECTION("increment"){
        IndexPriorityQueue<int> q {};
        q.insert(421, 2);
        q.insert(43145, 1);
        q.insert(3414, 3);
        q.insert(22, 221);
        q.insert(45, 4);
        
        q.increment(3414, 221);
        

        REQUIRE(q.peek() == 3414);
        REQUIRE(q.getValue(3414) == 3+221);
    }

    SECTION("increment negative number"){
        IndexPriorityQueue<int> q {};
        q.insert(421, 2);
        q.insert(43145, 1);
        q.insert(3414, 3);
        q.insert(22, 221);
        q.insert(45, 4);
        
        q.increment(22, -220);
        

        REQUIRE(q.peek() == 45);
        REQUIRE(q.getValue(22) == 1);
    }

    SECTION("increment minPQ"){
        IndexPriorityQueue<int> q {false};
        q.insert(421, 10);
        q.insert(315, 8);
        q.insert(3414, 9);
        q.insert(22, 4);
        q.insert(45, 5);
        
        REQUIRE(q.peek() == 22);
        q.increment(421, -8);
        REQUIRE(q.peek() == 421);

        q.increment(421, 3);
        REQUIRE(q.peek() == 22);

    }

    SECTION("getValue from content"){
        IndexPriorityQueue<int> q {};
        q.insert(421, 2);
        q.insert(43145, 1);
        q.insert(3414, 3);
        q.insert(22, 221);
        q.insert(45, 4);
        
        REQUIRE(q.getValue(3414) == 3);
        q.update(3414, 90);
        REQUIRE(q.getValue(3414) == 90);

    }

    SECTION("insert duplicate item only update "){
        IndexPriorityQueue<int> q {};
        q.insert(421, 60);
        q.insert(22, 50);
        REQUIRE(q.getSize() == 2);
        q.insert(421, 40);
        REQUIRE(q.getSize() == 2);
        REQUIRE(q.peek() == 22);
    }


    SECTION("integration test for insert, peek, pop, update"){
        IndexPriorityQueue<int> q {};
        q.insert(4, 8);
        q.insert(12, 2);
        q.insert(1, 7.7);
        q.insert(55, 3);
        q.insert(53, 9);

        REQUIRE(q.peek() == 53);
        REQUIRE(q.pop() == 53);

        REQUIRE(q.peek() == 4);
        q.insert(53, 9);
        REQUIRE(q.pop() == 53);
        q.insert(99, 100);
        REQUIRE(q.pop() == 99);
        REQUIRE(q.pop() == 4);
        q.insert(12, 5);
        q.insert(16, 0);
        q.insert(12,10);
        REQUIRE(q.pop() == 12);
        REQUIRE(q.pop() == 1);
        
    }

    SECTION("integration testing on min priority queue"){
        IndexPriorityQueue<int> q {false};
        q.insert(1,8.1);
        q.insert(2,6);
        q.insert(10,3);
        q.insert(11,3);
        q.insert(9,111);
        q.insert(223,90);
        REQUIRE(q.pop() == 10);
        REQUIRE(q.pop() == 11);
        q.insert(223, 0);
        REQUIRE(q.pop() == 223);
        q.update(9, 2);
        REQUIRE(q.pop() == 9);
    }

    SECTION("inQueue test"){
        IndexPriorityQueue<int> q {false};
        q.insert(1,8.1);
        q.insert(2,6);
        q.insert(10,3);
        q.insert(11,3);
        q.insert(9,111);
        q.insert(223,90);
        REQUIRE(q.inQueue(11));
        REQUIRE(q.pop() == 10);
        REQUIRE(q.pop() == 11);

        REQUIRE(!q.inQueue(10));
        REQUIRE(!q.inQueue(11));


        q.insert(223, 0);
        REQUIRE(q.pop() == 223);
        q.update(9, 2);

        REQUIRE(q.inQueue(9));
        REQUIRE(q.pop() == 9);
        REQUIRE(!q.inQueue(9));

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