/* Nurse scheduling day class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 08/02/2019
   Edited: 14/03/2019 */

#include <iostream>
#include <climits>

#include "include/Day.hpp"
#include "../util/include/util.hpp"

Day::Day(int n) : day_n(n){ }

void Day::getCostMatrix(int **cost) {
    int j = 0;
    int min_cost, min_shift, shift_cost;
    bool has_hard_restriction;

    for(int i = 0; i < problem.n_nurses; i++) {
        total_shifts = 0;
        min_cost = INT_MAX;
        min_shift = 0;

        for (j = 0; j < problem.n_shifts; j++) {
            has_hard_restriction = false;
            shift_cost = problem.nurses[i]->preferences[problem.n_shifts * day_n + j];

            if(!problem.nurses[i]->current_state->assgined_shifts.empty()) { // Hard constraints check
                if (problem.nurses[i]->current_state->assgined_shifts.back()->type == 1) {
                    if(j == 0){
                        shift_cost = HARD_CONSTRAINT_COST;
                        has_hard_restriction = true;
                    }
                }
                else if(problem.nurses[i]->current_state->assgined_shifts.back()->type == 2){
                    if((j == 0) || (j == 1)){
                        shift_cost = HARD_CONSTRAINT_COST;
                        has_hard_restriction = true;
                    }
                }
            }

            if(!has_hard_restriction){ // If there is a hard restriction, there is no need to see if there is a soft
                if((j != 3) && ((problem.nurses[i]->current_state->days_worked + 1) > problem.nsp_case.max_asg))
                    shift_cost = SOFT_CONSTRAINT_COST;

                if((j == 3) && (problem.nurses[i]->current_state->days_worked < problem.nsp_case.min_asg)) {
                    if (!problem.nurses[i]->current_state->reached_min_worked);
                        //shift_cost = shift_cost + 1; // If the minimum work days hasn't been achived, increase the cost of a free shift
                }

                if(!problem.nurses[i]->current_state->assgined_shifts.empty()){
                    if(problem.nurses[i]->current_state->assgined_shifts.back()->type == j){
                        if(j != 3){
                            if((problem.nurses[i]->current_state->con_working_shifts + 1) > problem.nsp_case.max_cws)
                                shift_cost = SOFT_CONSTRAINT_COST;
                            if(problem.nurses[i]->current_state->con_working_shifts < problem.nsp_case.min_cws) {
                                if (!problem.nurses[i]->current_state->reached_min_con_working)
                                    shift_cost = shift_cost - 1; // If the minimum hasn't been achived, decrease the cost
                            }
                        }

                        if((problem.nurses[i]->current_state->con_same_working_shifts + 1) >
                           problem.nsp_case.con_same_shift[j].second)
                            shift_cost = SOFT_CONSTRAINT_COST;
                    }

                    if(problem.nurses[i]->current_state->con_same_working_shifts <
                       problem.nsp_case.con_same_shift[j].first) {
                        if(!problem.nurses[i]->current_state->reached_min_same_assign[j])
                            shift_cost = shift_cost - 1;
                    }

                    if((problem.nurses[i]->current_state->number_assignments_shifts[j] + 1) >
                             problem.nsp_case.assg_shift[j].second)
                        shift_cost = SOFT_CONSTRAINT_COST;
                    if(problem.nurses[i]->current_state->number_assignments_shifts[j] <
                            problem.nsp_case.assg_shift[j].first) {
                        if(!problem.nurses[i]->current_state->reached_min_assign[j])
                            shift_cost = shift_cost - 1;
                    }
                }
            }

            if(shift_cost <= min_cost){ // Finds minimun cost shift
                min_cost = shift_cost;
                min_shift = j;
            }

            total_shifts = total_shifts + (j == 0 ? 0 : nurses_by_shift[j-1]);
            for(int k = 0; k < nurses_by_shift[j]; k++)
                cost[i][k + total_shifts] = shift_cost;
        }

        problem.nurses[i]->min_shift_now = min_shift;
        j = total_shifts;

        for (; j < problem.n_nurses; j++)
            cost[i][j] = min_cost;
    }

}

void Day::assignNurses(int* &assignments) {
    for(int i = 0; i < problem.n_nurses; i++){
        if(assignments[i] > (total_shifts -1))
            shifts[assignments[i]]->type = problem.nurses[i]->min_shift_now;

        problem.nurses[i]->assignShift(shifts[assignments[i]]);
    }
}

Day::~Day() {
    for (Shift *shift : shifts) delete shift;
}

void Day::unassignNurses() {
    for(Nurse* nurse : problem.nurses) nurse->getPrevState();
}
