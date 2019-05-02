/* Nurse scheduling case class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 23/11/2018
   Edited: 23/11/2018 */

#ifndef NSP_CASE_HPP
#define NSP_CASE_HPP

#include <vector>

class Case {
public:
    int min_asg = 0, max_asg = 0, min_cws = 0, max_cws = 0;
    std::vector<std::pair<int, int>> con_same_shift;
    std::vector<std::pair<int, int>> assg_shift;

    /**
     * Prints the case information to the console
     */
    void printCase();
};


#endif /* NSP_CASE_HPP */
