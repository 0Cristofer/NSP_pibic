/* Nurse scheduling problem class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 23/11/2018
   Edited: 14/03/2019 */

#ifndef NSP_NSP_HPP
#define NSP_NSP_HPP

#define HARD_CONSTRAINT_COST 100
#define SOFT_CONSTRAINT_COST 100

#include "Case.hpp"
#include "Nurse.hpp"
#include "Day.hpp"
#include "Solution.hpp"

class NSP {
public:
    int n_days = 0, n_nurses = 0, n_shifts = 0, total_cost = 0, k_sol = 0, max_depth = 0;
    int current_day = 0, last_depth_day = 0, **cost;

    Case nsp_case;

    std::vector<Day*> days;
    std::vector<Nurse*> nurses;

    /**
     * Starts this problem solving
     */
    void solve();

    /**
     * Solves a single day, calling it recursively to each solution found. Handles all the backtracking algorithm
     * @param day The day to be solved
     * @param assignments The previous' day assignments
     * @param solution_cost Reference return. Is modified to return the total solution cost.
     * @return The solution instance of this day.
     */
    Solution* solveDay(Day *day, int *assignments, int *solution_cost);

    /**
     * Prints the problem's information
     */
    void printProblem();

    virtual ~NSP();
};


#endif /* NSP_NSP_HPP */
