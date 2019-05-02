/* Nurse scheduling solution class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 14/03/2019
   Edited: 14/03/2019 */

#include <cstdlib>
#include "include/Solution.hpp"


Solution::~Solution() {
    clear();
}

void Solution::clear() {
    if(solution != nullptr) free(solution);
    if(next != nullptr) delete (next);

    solution = nullptr;
    next = nullptr;
}
