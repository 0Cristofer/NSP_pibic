/* Nurse scheduling nurse class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 23/11/2018
   Edited: 16/02/2019 */

#include <iostream>
#include "include/Nurse.hpp"
#include "include/Shift.hpp"
#include "../util/include/util.hpp"

Nurse::Nurse(int n) : number(n) {
    current_state = new State();
}

void Nurse::assignShift(Shift *shift){
    newState();

    if(shift->type != 3) {
        current_state->days_worked = current_state->days_worked + 1;
        current_state->con_working_shifts = current_state->con_working_shifts + 1;

        if(current_state->days_worked >= problem.nsp_case.min_asg)
            current_state->reached_min_worked = true;
        if(current_state->con_working_shifts >= problem.nsp_case.min_cws)
            current_state->reached_min_con_working = true;
    }
    else current_state->con_working_shifts = 0;

    if(!current_state->assgined_shifts.empty()) {
        if (current_state->assgined_shifts.back()->type == shift->type) {
            current_state->con_same_working_shifts = current_state->con_same_working_shifts + 1;
        }
        else{
            current_state->con_same_working_shifts = 1;
        }
    }
    else{
        current_state->con_same_working_shifts = 1;
    }

    if(current_state->con_same_working_shifts >= problem.nsp_case.con_same_shift[shift->type].first)
        current_state->reached_min_same_assign[shift->type] = true;

    current_state->number_assignments_shifts[shift->type] = current_state->number_assignments_shifts[shift->type] + 1;

    if(current_state->number_assignments_shifts[shift->type] >= current_state->number_assignments_shifts[shift->type])
        current_state->reached_min_assign[shift->type] = true;

    current_state->assgined_shifts.push_back(shift);
    shift->nurse = this;
}

void Nurse::newState() {
    current_state = new State(current_state);
}

void Nurse::getPrevState() {
    State* prev = current_state->prev;

    delete(current_state);

    current_state = prev;
}

Nurse::~Nurse() {
    State* current = current_state;

    while(current != nullptr){
        current_state = current->prev;

        delete(current);

        current = current_state;
    }

    delete(current_state);
}

int Nurse::getNumConstraintsViolated() {
    int total = 0;

    if(!problem.nsp_case.min_asg)
        current_state->reached_min_worked = true;

    if(!problem.nsp_case.min_cws)
        current_state->reached_min_con_working = true;

    if(!current_state->reached_min_worked){
        if(args.verbose) std::cout << "Nurse: " << number << " didn't reach minimum worked" << std::endl;
        total = total + 1;
    }
    if(!current_state->reached_min_con_working){
        if(args.verbose) std::cout << "Nurse: " << number << " didn't reach minimum consecutive working" << std::endl;
        total = total + 1;
    }

    for(int i = 0; i < problem.n_shifts; i++){
        if(!problem.nsp_case.con_same_shift[i].first)
            current_state->reached_min_same_assign[i] = true;

        if(!problem.nsp_case.assg_shift[i].first)
            current_state->reached_min_assign[i] = true;

        if(!current_state->reached_min_same_assign[i]){
            if(args.verbose) std::cout << "Nurse: " << number << " didn't reach minimum same assignment of: " << i << std::endl;
            total = total + 1;
        }
        if(!current_state->reached_min_assign[i]){
            if(args.verbose) std::cout << "Nurse: " << number << " didn't reach minimum assignment" << std::endl;
            total = total + 1;
        }
    }

    return total;
}

State::State(State *state): days_worked(state->days_worked), con_same_working_shifts(state->con_same_working_shifts),
                            con_working_shifts(state->con_working_shifts), reached_min_worked(state->reached_min_worked),
                            reached_min_con_working(state->reached_min_con_working), prev(state){
    number_assignments_shifts.assign(state->number_assignments_shifts.begin(), state->number_assignments_shifts.end());
    assgined_shifts.assign(state->assgined_shifts.begin(), state->assgined_shifts.end());
    reached_min_assign.assign(state->reached_min_assign.begin(), state->reached_min_assign.end());
    reached_min_same_assign.assign(state->reached_min_same_assign.begin(), state->reached_min_same_assign.end());
}