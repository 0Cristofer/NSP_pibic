/* Nurse scheduling day class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 07/02/2019
   Edited: 14/03/2019 */

#ifndef NSP_DAY_HPP
#define NSP_DAY_HPP

#include <vector>
#include "Shift.hpp"

class Day {
public:
    int day_n;
    int total_shifts = 0;
    std::vector<int> nurses_by_shift;
    std::vector<Shift*> shifts;

    /***
     * Constructs a day
     * @param n This day's number
     */
    explicit Day(int n);

    /***
     * Generates the assignment cost matrix of this day
     * @param cost The matrix to be filled
     */
    void getCostMatrix(int** cost);

    /***
     * Assign all nurses to this day's shifts
     * @param assignments A vector where nurse i is assigned to shift v[i]
     */
    void assignNurses(int* &assignments);

    /**
     * Reset this day nurses assignments
     */
    static void unassignNurses();

    virtual ~Day();
};


#endif //NSP_DAY_HPP
