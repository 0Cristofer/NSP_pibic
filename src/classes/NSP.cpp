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
#include "../apc/include/apc.h"

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
    int solution_cost = 0;
    Solution* solution, *current;

    cost = (int **) std::malloc(problem.n_nurses * sizeof(int*));

    for(int i = 0; i < problem.n_nurses; i++)
        cost[i] = (int *) std::malloc(problem.n_nurses * sizeof(int));

    solution = solveDay(days.front(), nullptr, &solution_cost);
    current = solution;

    // Based on the linked list solution, reassign all days/nurses
    for(Day* day: days){
        day->getCostMatrix(cost);

        day->assignNurses(current->solution);
        current = current->next;
    }

    // Gets the final solution cost based on the assignments
    solution_cost = 0;
    for(Day* day: days){
        for(Shift *shift : day->shifts){
            solution_cost = solution_cost + shift->nurse->preferences[problem.n_shifts * day->day_n + shift->type];
        }
    }

    this->total_cost = solution_cost;

    std::cout << "Problem solved. Total cost: " << this->total_cost << std::endl;

    // DEBUG START
    bool check = true;
    for(Nurse *nurse : nurses){
        check = nurse->assertConstraints() && check;
    }
    assert(check);
    // DEBUG END

    delete(solution);

    for(int j = 0; j < n_nurses; j++) std::free(cost[j]);
    std::free(cost);
}

Solution * NSP::solveDay(Day *day, int *assignments, int *solution_cost) {
    int solution_size, next_solution_cost = 0, min_next_cost = INT_MAX;
    int **solutions;

    // DEBUG START
    int **cos;
#define s 5
    int c[s][s] = {
            {2, 3, 4, 4, 5},
            {4, 1, 4, 3, 3},
            {4, 2, 1, 2, 2},
            {3, 3, 3, 1, 5},
            {3, 3, 3, 1, 3}

    };

    cos = (int **) malloc(sizeof(int*) * s);
    for(int i = 0; i < s; i++)
        cos[i] = (int *) malloc(sizeof(int) * s);

    for(int i = 0; i < s; i++)
        for(int j = 0; j < s; j++)
            cos[i][j] = c[i][j];
    // DEBUG END

    Solution* current = nullptr, *next_solution = nullptr;

    if(assignments != nullptr) days[current_day - 1]->assignNurses(assignments); // The first day won't have assignments

    if(day != nullptr){ // The last day won't be a real day, just a assignment
        current = new Solution();

        // Temporário, a alocação das soluções será feita pelo APC
        solutions = (int **) malloc(sizeof(int*));
        *solutions = (int *) malloc(n_nurses * sizeof(int));

        day->getCostMatrix(cost);

        // DEBUG START
        for(int i = 0; i < problem.n_nurses; i++){
            for(int j = 0; j < problem.n_nurses; j++){
                std::cout << cost[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        // DEBUG END

        apc(problem.n_nurses, cost, INT_MAX, solution_cost, *solutions, &solution_size);

        assert(*solution_cost < SOFT_CONSTRAINT_COST); // Hard/soft max constraints check

        for(int i = 0; i < solution_size; i++){ // For each solution found
            current_day = current_day + 1;

            // Solve the next day based on the current solution
            if(current_day < n_days) next_solution = solveDay(days[current_day], solutions[i], &next_solution_cost);
            else solveDay(nullptr, solutions[i], &next_solution_cost); // For the last day

            if(next_solution_cost < min_next_cost){ // Finds the lowest overall cost solution
                current->clear();

                current->next = next_solution;
                current->solution = solutions[i];
                min_next_cost = next_solution_cost;
            }
            else{
                free(solutions[i]);
                delete(next_solution);
            }

            Day::unassignNurses();
        }

        *solution_cost = *solution_cost + min_next_cost;

        free(solutions);
    }

    // DEGUG START
    for(int i = 0; i < s; i++)
        free(cos[i]);
    free(cos);
    // DEBUG END

    current_day = current_day - 1;

    return current;
}

NSP::~NSP() {
    for (Day *day : days) delete day;
    for (Nurse *nurse: nurses) delete nurse;
}
