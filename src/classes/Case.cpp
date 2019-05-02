/* Nurse scheduling case class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 23/11/2018
   Edited: 23/11/2018 */

#include <iostream>
#include "include/Case.hpp"

void Case::printCase() {
    std::cout << "Case information:" << std::endl;
    std::cout << "\tMin assignments: " << min_asg << ", Max assignments: " << max_asg << std::endl;
    std::cout << "\tMin consecutive shifts: " << min_cws << ", Max consecutive shifts: " << max_cws << std::endl;

    std::cout << "\tConsecutive same working shifts (min - max):" << std::endl;
    for(auto p: con_same_shift){
        std::cout << "\t\t" << p.first << " - " << p.second << std::endl;
    }

    std::cout << "\tAssignments per shift (min - max):" << std::endl;
    for(auto p: assg_shift){
        std::cout << "\t\t" << p.first << " - " << p.second << std::endl;
    }
}
