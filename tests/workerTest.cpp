#include "../include/catch_amalgamated.hpp"
#include "../include/worker.hpp"
#include <stdexcept>
	

TEST_CASE("worker class basic functions", "[worker]"){
    SECTION("construction"){
        std::string name {"hello world"};
        Worker w {name, 10, 0};
        REQUIRE(w.getName() == name);
        REQUIRE(w.getId() == 0);
        REQUIRE(w.getDesiredHours() == 10);
        REQUIRE(w.getAllocatedHOurs() == 0 );
    }

    SECTION("setters and getters"){
        std::string name {"hello world"};
        Worker w {name, 10, 0};
        w.setDesiredHours(15);
        REQUIRE(w.getDesiredHours() == 15);
    }

    SECTION("allocated hours"){
        std::string name {"hello world"};
        Worker w {name, 10, 0};
        w.allocatHours(5.5, true);
        REQUIRE(w.getAllocatedHOurs() == 5.5);
        REQUIRE(w.getAllocatedPreferedHours() == 5.5);
        w.allocatHours(2, false);
        REQUIRE(w.getAllocatedHOurs() == 7.5);
        REQUIRE(w.getAllocatedPreferedHours() == 5.5);

        try
        {
            w.allocatHours(-5, true);
            REQUIRE(false);
        }
        catch(const std::invalid_argument& e)
        {
            REQUIRE(true);
        }
        
    }


}