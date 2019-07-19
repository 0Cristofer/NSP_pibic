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
    delete solution;
    delete next;

    solution = nullptr;
    next = nullptr;
}

void Solution::setSolution(std::vector<int> *sol) {
    solution = new std::vector<int>();

    solution->insert(solution->end(), sol->begin(), sol->end());
}
