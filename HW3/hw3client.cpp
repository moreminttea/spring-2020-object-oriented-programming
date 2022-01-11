// ecs36b first program

#include <iostream>
#include "Shadow_Flight.h"
#include "Shadow_Person.h"

using namespace std;

int main()
{
    Shadow_Person sp1 { "http://localhost:8384", "1234567890", "Person", "00000005" };
    std::cout << ((Person&) sp1).getVsID() << std::endl;
    Shadow_Person sp2 { "http://localhost:8384", "1234567890", "Person", "00000006" };
    std::cout << ((Person&) sp2).getVsID() << std::endl;
    Shadow_Person sp3 { "http://localhost:8385", "1234567890", "Person", "00000006" };
    std::cout << ((Person&) sp3).getVsID() << std::endl;
    
    Shadow_Flight sf3 { "http://localhost:8384", "1234567890", "Flight", "00000003" };
    std::cout << std::to_string(((Flight&) sf3).getDistance()) << std::endl;
    Shadow_Flight sf2 { "http://localhost:8384", "1234567890", "Flight", "00000002" };
    std::cout << std::to_string(sf2.getDistance()) << std::endl;
    Shadow_Flight sf1 { "http://localhost:8384", "1234567890", "Flight", "00000001" };
    std::cout << std::to_string(sf1.getDistance()) << std::endl;
    
    if (sp1 == sp2) printf("They are equal\n");
    if (sp1 == sp3) printf("They are equal\n");
    if (sf1.Conflict_of_Interest()) printf("sf1 coi\n");
    if (sf2.Conflict_of_Interest()) printf("sf2 coi\n");
    if (sf3.Conflict_of_Interest()) printf("sf3 coi\n");
    
    return 0;
}
