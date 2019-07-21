/* Nurse scheduling problem class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 23/11/2018
   Edited: 16/04/2019 */

#include <iostream>
#include <climits>

//#define NDEBUG
#include <assert.h>

#include "include/NSP.hpp"
#include "../util/include/util.hpp"
#include "../apc/include/apc.hpp"

void NSP::printProblem() {
    std::cout << "---------------------------------" << std::endl;

    std::cout << "Problem:" << std::endl;
    std::cout << R"(    Number of nurses: )" << n_nurses << std::endl;
    std::cout << R"(    Number of days: )" << n_days << std::endl;
    std::cout << R"(    Number of shifts: )" << n_shifts << std::endl << std::endl;

    nsp_case.printCase();

    std::cout << "---------------------------------" << std::endl;
}

void NSP::solve() {
    int solution_cost, total = 0;
    Solution *solution = nullptr, *current;

    cost = (int **) std::malloc(problem.n_nurses * sizeof(int*));

    for(int i = 0; i < problem.n_nurses; i++)
        cost[i] = (int *) std::malloc(problem.n_nurses * sizeof(int));


    while(current_day < n_days) {
        solution_cost = 0;
        total = 0;

        solution = solveDay(*(days.begin() + current_day), nullptr, &solution_cost);

        current_day = current_day + max_depth + 1;
        current = solution;

        // Based on the linked list solution, reassign all days/nurses
        if(current_day < n_days) {
            for (int i = last_depth_day; i < current_day; i++) {
                days[i]->getCostMatrix(cost);

                days[i]->assignNurses(current->solution->data());
                current = current->next;
            }
        }
        else{
            for (int i = last_depth_day; i < n_days; i++) {
                days[i]->getCostMatrix(cost);

                days[i]->assignNurses(current->solution->data());
                current = current->next;
            }
        }

        last_depth_day = current_day;
        delete solution;
    }


    // Gets the final solution cost based on the assignments
    solution_cost = 0;
    for(Day* day: days){
        for(Shift *shift : day->shifts){
            solution_cost = solution_cost + shift->nurse->preferences[problem.n_shifts * day->day_n + shift->type];
        }
    }

    this->total_cost = solution_cost;

    if(args.verbose)
        std::cout << "Problem solved. Total cost: " << this->total_cost << std::endl;
    else
        std::cout << total_cost << std::endl;

    for(Nurse *nurse : nurses){
        total = total + nurse->getNumConstraintsViolated();
    }

    if(args.verbose)
        std::cout << "Total soft constraints violated: " << total << std::endl;
    else
        std::cout << total << std::endl;

    for(int j = 0; j < n_nurses; j++) std::free(cost[j]);
    std::free(cost);
}

Solution * NSP::solveDay(Day *day, int *assignments, int *solution_cost) {
    int next_solution_cost = 0, min_next_cost = INT_MAX;
    int *single_solution;
    std::vector<std::vector<int> *> *solutions;

    Solution* current = nullptr, *next_solution = nullptr;

    if(assignments != nullptr) days[current_day - 1]->assignNurses(assignments); // The first day won't have assignments

    if(day != nullptr){ // The last day won't be a real day, just a assignment
        current = new Solution();

        single_solution = (int *) malloc(n_nurses * sizeof(int));

        day->getCostMatrix(cost);

        apc(problem.n_nurses, cost, INT_MAX, solution_cost, single_solution, &solutions, k_sol);

        for(auto & solution : *solutions){ // For each solution found
            current_day = current_day + 1;

            if((current_day - last_depth_day) < max_depth) {
                // Solve the next day based on the current solution
                if (current_day < n_days)next_solution = solveDay(days[current_day], solution->data(), &next_solution_cost);
                else solveDay(nullptr, solution->data(), &next_solution_cost); // For the last day

                if (next_solution_cost < min_next_cost) { // Finds the lowest overall cost solution
                    current->clear();

                    current->next = next_solution;
                    current->setSolution(solution);
                    min_next_cost = next_solution_cost;
                }
                else {
                    delete next_solution;
                }

                Day::unassignNurses();
            }
            else{
                current_day = current_day - 1;
                current->next = nullptr;
                current->setSolution(solution);
                min_next_cost = 0;
                break;
            }
        }

        *solution_cost = *solution_cost + min_next_cost;

        for(std::vector<int> *solution : *solutions) delete solution;
        delete solutions;
        free(single_solution);
    }

    current_day = current_day - 1;

    return current;
}

NSP::~NSP() {
    for (Day *day : days) delete day;
    for (Nurse *nurse: nurses) delete nurse;
}
