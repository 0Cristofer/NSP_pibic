/* Nurse scheduling solution class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 14/03/2019
   Edited: 14/03/2019 */

#ifndef NSP_SOLUTION_HPP
#define NSP_SOLUTION_HPP

#include <vector>

/**
 * Represents a single day solution, having a reference to the next
 */
class Solution {
public:
    Solution() = default;
    ~Solution();

    std::vector<int> *solution = nullptr;
    Solution* next = nullptr;

    void setSolution(std::vector<int> *sol);
    void clear();

};


#endif //NSP_SOLUTION_HPP
